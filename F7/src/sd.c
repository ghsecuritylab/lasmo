#include "F7/sd.h"

#include <string.h>
#include "ch.h"

static char test_file_path[10] = "Test.txt";

///////////// SD slot monitor

#define POLLING_INTERVAL                10
#define POLLING_DELAY                   10

// Poll timer
static virtual_timer_t sd_slot_poll_timer;

// Debounce counter
static unsigned sd_slot_debounce_counter;

// Card event sources
static event_source_t card_inserted, card_removed;

// Handler for the poll timer
static void poll_timer_handler(void *p) {
  BaseBlockDevice *bbdp = p;

  chSysLockFromISR();
  if (sd_slot_debounce_counter > 0) {
    if (blkIsInserted(bbdp)) {
      if (--sd_slot_debounce_counter == 0) {
        chEvtBroadcastI(&card_inserted);
      }
    }
    else
      sd_slot_debounce_counter = POLLING_INTERVAL;
  }
  else {
    if (!blkIsInserted(bbdp)) {
      sd_slot_debounce_counter = POLLING_INTERVAL;
      chEvtBroadcastI(&card_removed);
    }
  }
  chVTSetI(&sd_slot_poll_timer, TIME_MS2I(POLLING_DELAY), poll_timer_handler, bbdp);
  chSysUnlockFromISR();
}

///////////// Read/write functions

#define BUFF_SIZE 1024

// FS object
static FATFS SDC_FS;

// Availability of the filesystem
static int fs_ready;

//Taken from the RT-STM32-USB-FATFS demo
/* Generic large buffer.*/
static uint8_t fbuff[BUFF_SIZE];

static char to_esp32[ILDA_PATH_MAX_SIZE];

static FRESULT scan_files(char *path) {
  static FILINFO fno;
  FRESULT res;
  DIR dir;
  size_t i;
  char *fn;

  res = f_opendir(&dir, path);
  if (res == FR_OK) {
    i = strlen(path);
    while (((res = f_readdir(&dir, &fno)) == FR_OK) && fno.fname[0]) {
      if (FF_FS_RPATH && fno.fname[0] == '.')
        continue;
      fn = fno.fname;
      if (fno.fattrib & AM_DIR) {
        *(path + i) = '/';
        strcpy(path + i + 1, fn);
        res = scan_files(path);
        *(path + i) = '\0';
        if (res != FR_OK)
          break;
      }
      else {
        SEGGER_RTT_printf(0, "%s/%s\n", path, fn);
      }
    }
  }
  return res;
}

static int scan_files_in_buff(char* path, int* depth) {
  static FILINFO fno;
  int res;
  DIR dir;
  size_t i;
  char *fn;

  res = f_opendir(&dir, path);
  if (res == FR_OK) {
    i = strlen(path);
    while (((res = f_readdir(&dir, &fno)) == FR_OK) && fno.fname[0]) {
      if (FF_FS_RPATH && fno.fname[0] == '.')
        continue;
      fn = fno.fname;
      if (fno.fattrib & AM_DIR) {
        *depth += 1;
        *(path + i) = '/';
        strcpy(path + i + 1, fn);
        res = scan_files_in_buff(path, depth);
        *(path + i) = '\0';
        *depth -= 1;
      }
      else {
        if(strstr(fn, ".ild")) {
          // need to check if the extension is really '.ild' without anything after
          if(strstr(fn, ".ild")[4] == '\0') {
            strcat(strcat(strcat(strcat(to_esp32, path), "/"), fn), "");
            lsm_uart_send_ilda_path(to_esp32);
            to_esp32[0] = '\0';
          }
        }
      }
    }

    SEGGER_RTT_printf(0, "Info: Leaving directory, current depth: %d\n", *depth);

    if(*depth == 0)
      SEGGER_RTT_printf(0, "lsm_sd_send_tree_to_esp: Info: The tree has been sent.\n");

  }
  return res;
}
// Print the tree of the SD card
void lsm_sd_print_tree(void){
  FRESULT err;
  uint32_t fre_clust;
  FATFS *fsp;

  if(!fs_ready) {
    SEGGER_RTT_WriteString(0, "Filesystem not ready.");
    return;
  }

  err = f_getfree("/", &fre_clust, &fsp);
  if (err != FR_OK) {
    SEGGER_RTT_printf(0, "FileSystem error. Check your disk. Error: %d", err);
    return;
  }

  fbuff[0] = 0;
  scan_files((char*)fbuff);
}

void lsm_sd_send_tree_to_esp(void) {
  uint32_t fre_clust;
  FATFS *fsp;

  if(!fs_ready) {
    SEGGER_RTT_WriteString(0, "Filesystem not ready.");
    return;
  }

  FRESULT err;

  if((err = f_getfree("/", &fre_clust, &fsp)) != FR_OK) {
    SEGGER_RTT_printf(0, "Filesystem error. Check your disk. Error: %d\n", err);
    return;
  }

  fbuff[0] = 0;
  int depth = 0;
  scan_files_in_buff((char*)fbuff, &depth);
}

////////////////// User thread and functions

static THD_WORKING_AREA(lsm_wa_sd_thread, 4096);

static THD_FUNCTION(lsm_sd_thread, arg) {
  (void)arg;

  event_listener_t insert_card_listener, remove_card_listener;

  chEvtRegisterMask(&card_inserted, &insert_card_listener, EVENT_MASK(0));
  chEvtRegisterMask(&card_removed,  &remove_card_listener, EVENT_MASK(1));

  while(true){
    eventmask_t evt = chEvtWaitOne(ALL_EVENTS);

    if(evt & EVENT_MASK(0))
      lsm_sd_insert_handler(0);
    else if(evt & EVENT_MASK(1))
      lsm_sd_remove_handler(0);
  }
}

void lsm_sd_insert_handler(eventid_t id){
  FRESULT err;
  (void)id;

  if (sdcConnect(&SDCD1)) {
    SEGGER_RTT_WriteString(0, "Connection failure (failure in sdcConnect)\n");
    return;
  }

  SEGGER_RTT_WriteString(0, "Carte SD insérée!\n");

  err = f_mount(&SDC_FS, "/", 1);
  if (err != FR_OK) {
    SEGGER_RTT_WriteString(0, "Mount error\n");
    sdcDisconnect(&SDCD1);
    return;
  }

  fs_ready = 1;

  return;
}

void lsm_sd_remove_handler(eventid_t id){
  (void)id;
  sdcDisconnect(&SDCD1);
  fs_ready = 0;
  SEGGER_RTT_WriteString(0, "Carte SD déconnectée!\n");
}

UINT lsm_sd_write_file(FIL* fp, void* data, int datalen){
  FRESULT err;
  UINT bytes_written;

  err = f_write(fp, data, datalen, &bytes_written);
  chThdSleepMilliseconds(512);


  if(err){
    //SEGGER_RTT_printf(0, "lsm_sd_write_file: Write error: error #%d\n", err);
    return 0;
  } else {
    //SEGGER_RTT_printf(0, "lsm_sd_write_file: Info: Successfully wrote %d bytes to test file.\n", bytes_written);
    return bytes_written;
  }

}


int lsm_sd_read_file(lsm_ilda_file_t* fp, char* buff, int buflen){
  FRESULT err;
  UINT bytes_read;

  err = f_read(&(fp->orig_file), buff, buflen, &bytes_read);
  if(bytes_read == 0)
    SEGGER_RTT_WriteString(0, "lsm_sd_read_file: Info: End of file reached!\n");

  if(err) {
    SEGGER_RTT_printf(0, "lsm_sd_read_file: Read error: error #%d\n", err);
    return 0;
  }

  return (int) bytes_read;

}

int lsm_sd_read_file_with_offset(lsm_ilda_file_t* fp, char* buff, int buflen, FSIZE_t offset){
  FRESULT err;

  err = f_lseek(&(fp->orig_file), offset);
  if(err){
    SEGGER_RTT_printf(0, "lsm_sd_read_file_with_offset: Error: Invalid offset\n");
    return 0;
  }

  return lsm_sd_read_file(fp, buff, buflen);
}

int lsm_sd_open_file(lsm_ilda_file_t* fp, const char* path) {
  FRESULT err;

  fp->flags |= LSM_ILDA_FROM_SD;

  err = f_open(&(fp->orig_file), path, FA_READ | FA_OPEN_EXISTING);

  if(err != FR_OK) {
    if(err == FR_NO_FILE)           SEGGER_RTT_printf(0, "lsm_sd_open_file: File Error: File not found, add file \'%s\' into your root directory on the SD card.\n", path);
    else if(err == FR_INVALID_NAME) SEGGER_RTT_WriteString(0, "lsm_sd_open_file: File Error: Invalid path\n");
    else if(err == FR_NO_PATH)      SEGGER_RTT_WriteString(0, "lsm_sd_open_file: File Error: Directory not found\n");
    else                            SEGGER_RTT_printf(0, "lsm_sd_open_file: File error: #%d\n", err);
    return err;
  }
  return 0;
}

int lsm_sd_close_file(lsm_ilda_file_t* fp) {
  if(!(fp->flags && LSM_ILDA_FROM_SD)) {
    SEGGER_RTT_printf(0, "lsm_sd_open_file: File error: this ILDA file is not on the SD card\n");
    return -1;
  }
  return (int) f_close(&(fp->orig_file));
}

/// Test area

static THD_WORKING_AREA(lsm_wa_sd_test_thread, 4096);

static THD_FUNCTION(lsm_sd_test_thread, arg){
  (void)arg;

  SEGGER_RTT_printf(0, "lsm_sd_test: Info: Printing tree of the SD card...\n");

  lsm_sd_print_tree();

  SEGGER_RTT_WriteString(0, "\n");

  SEGGER_RTT_WriteString(0, "lsm_sd_test: Info: Reading some data on the device...\n");

  int bytes_read;
  static lsm_ilda_file_t test_file;
  static char buff[BUFF_SIZE];

  test_file.flags = LSM_ILDA_FROM_SD;

  if(lsm_sd_open_file(&test_file, test_file_path))
    chThdExit(1);

  while (1) {
    bytes_read = lsm_sd_read_file(&test_file, buff, BUFF_SIZE-1); // BUFF_SIZE-1 because we need space for the null character
    if(bytes_read == 0) break;
    buff[bytes_read] = '\0'; //we end the string
    SEGGER_RTT_printf(0, "%s\n", buff);
  }

  lsm_sd_close_file(&test_file);

  // Testing the write function without using the lsm_ilda_file_t structure, which is meant to be read_only

  SEGGER_RTT_printf(0, "lsm_sd_test: Info: Writing some data on the device...\n");
  FIL test_file_write;

  f_open(&test_file_write, "Test2.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
  lsm_sd_write_file(&test_file_write, "&random_nb", 80);
  f_close(&test_file_write);

  chThdExit(0);
}

void lsm_sd_test(void){
  lsm_sd_init();
  if(!fs_ready){
    SEGGER_RTT_printf(0, "lsm_sd_test: Carte SD non connectée.\n");
    return;
  }

  SEGGER_RTT_printf(0, "lsm_sd_test: Starting test...\n");

  chThdWait(chThdCreateStatic(lsm_wa_sd_test_thread, sizeof(lsm_wa_sd_test_thread), NORMALPRIO + 7, lsm_sd_test_thread, NULL));

  return;

}

static bool sd_init = false;
// Initialisation function
void lsm_sd_init(void){
  if(!sd_init){
    sdcStart(&SDCD1, NULL);

    chEvtObjectInit(&card_inserted);
    chEvtObjectInit(&card_removed);
    SEGGER_RTT_printf(0,"sd init\n");

    chThdCreateStatic(lsm_wa_sd_thread, sizeof(lsm_wa_sd_thread), NORMALPRIO +2, lsm_sd_thread, NULL);
    SEGGER_RTT_printf(0,"sd init2\n");

    if(sdc_lld_is_card_inserted(&SDCD1))
      lsm_sd_insert_handler(0);

    chSysLock();
    sd_slot_debounce_counter = 0;
    chVTSetI(&sd_slot_poll_timer, TIME_MS2I(POLLING_DELAY), poll_timer_handler, &SDCD1);
    chSysUnlock();
    sd_init = true;
  }
}

int lsm_is_sd_connected(void){
  return fs_ready;
}
