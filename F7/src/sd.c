#include "F7/sd.h"

#include <string.h>
#include "ch.h"

///////////// SD slot monitor

#define POLLING_INTERVAL                10
#define POLLING_DELAY                   10

#define TEST_FILE                       "Test.txt"

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
int fs_ready;

//Taken from the RT-STM32-USB-FATFS demo
/* Generic large buffer.*/
static uint8_t fbuff[BUFF_SIZE];

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
        SEGGER_RTT_printf(0, "%s/%s\r\n", path, fn);
      }
    }
  }
  return res;
}

// Print the tree rooted at the specified path on the SD card
void lsm_print_tree(void){
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
      lsm_insert_handler(0);
    else if(evt & EVENT_MASK(1))
      lsm_remove_handler(0);
  }
}

void lsm_insert_handler(eventid_t id){
  FRESULT err;
  (void)id;

  if (sdcConnect(&SDCD1)) {
    SEGGER_RTT_WriteString(0, "Connection failure (failure in sdcConnect)\r\n");
    return;
  }

  SEGGER_RTT_WriteString(0, "Carte SD insérée!\r\n");

  err = f_mount(&SDC_FS, "/", 1);
  if (err != FR_OK) {
    SEGGER_RTT_WriteString(0, "Mount error\r\n");
    sdcDisconnect(&SDCD1);
    return;
  }

  fs_ready = 1;

  lsm_sd_test();

  return;
}

void lsm_remove_handler(eventid_t id){
  (void)id;
  sdcDisconnect(&SDCD1);
  fs_ready = 0;
  SEGGER_RTT_WriteString(0, "Carte SD déconnectée!\r\n");
}

UINT lsm_sd_write_file(FIL* fp, void* data, int datalen){
  FRESULT err;
  UINT bytes_written;

  err = f_write(fp, data, datalen, &bytes_written);
  chThdSleepMilliseconds(512);

  static char test_write[80];

  f_lseek(fp, 0);
  lsm_sd_read_file(fp, test_write, 80);
  SEGGER_RTT_printf(0, "Written on the file : %s\r\n", test_write);

  if(err){
    SEGGER_RTT_printf(0, "lsm_sd_write_file: Write error: error #%d\r\n", err);
    return 0;
  } else {
    SEGGER_RTT_printf(0, "lsm_sd_write_file: Info: Successfully wrote %d bytes to test file.\r\n", bytes_written);
    return bytes_written;
  }

}

UINT lsm_sd_read_file(FIL* fp, char* buff, int buflen){
  FRESULT err;
  UINT bytes_read;

  err = f_read(fp, buff, buflen, &bytes_read);
  if(bytes_read == 0)
    SEGGER_RTT_WriteString(0, "lsm_sd_read_file: Info: End of file reached!\r\n");

  if(err) {
    SEGGER_RTT_printf(0, "lsm_sd_read_file: Read error: error #%d\r\n", err);
    return 0;
  }

  return bytes_read;

}

UINT lsm_sd_read_file_with_offset(FIL* fp, char* buff, int buflen, FSIZE_t offset){
  FRESULT err;

  err = f_lseek(fp, offset);
  if(err){
    SEGGER_RTT_printf(0, "lsm_sd_read_file_with_offset: Error: Invalid offset\r\n");
    return 0;
  }

  return lsm_sd_read_file(fp, buff, buflen);
}

void lsm_sd_test(void){
  if(!fs_ready){
    SEGGER_RTT_printf(0, "lsm_sd_test: Carte SD non connectée.\r\n");
    return;
  }

  SEGGER_RTT_printf(0, "lsm_sd_test: Info: Printing tree of the SD card...\r\n");

  lsm_print_tree();

  SEGGER_RTT_WriteString(0, "\r\n");

  // Example of use of lsm_sd_read_file (reading a file on the SD card and printing it on the RTT)
  FRESULT err;
  FIL test_file;
  FIL test_file_write;
  UINT bytes_read;
  FSIZE_t offset = 0;
  static char buff[BUFF_SIZE];

  err = f_open(&test_file, TEST_FILE, FA_READ | FA_OPEN_EXISTING);

  if(err != FR_OK) {
    if(err == FR_NO_FILE)           SEGGER_RTT_WriteString(0, "lsm_sd_read_file: File Error: File not found, add file \'"TEST_FILE"\' into your root directory on the SD card.\r\n");
    else if(err == FR_INVALID_NAME) SEGGER_RTT_WriteString(0, "lsm_sd_read_file: File Error: Invalid path\r\n");
    else if(err == FR_NO_PATH)      SEGGER_RTT_WriteString(0, "lsm_sd_read_file: File Error: Directory not found\r\n");
    else                            SEGGER_RTT_printf(0, "lsm_sd_read_file: File error: #%d\r\n", err);
    return;
  }

  err = f_open(&test_file_write, "Test2.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);

  lsm_sd_write_file(&test_file_write, "&random_nb", 80);

  f_close(&test_file_write);

  while (1) {
    bytes_read = lsm_sd_read_file_with_offset(&test_file, buff, BUFF_SIZE-1, offset); // BUFF_SIZE-1 because we need space for the null character
    if(bytes_read == 0) break;
    buff[bytes_read] = '\0'; //we end the string
    SEGGER_RTT_WriteString(0, buff);
    chThdSleepMilliseconds(1); // necessary to avoid RTT buffer overflow (which causes a hard fault)
    offset = offset + (FSIZE_t) bytes_read;
  }
}

// Initialisation function
void lsm_sd_init(void){
    sdcStart(&SDCD1, NULL);

    chEvtObjectInit(&card_inserted);
    chEvtObjectInit(&card_removed);

    chThdCreateStatic(lsm_wa_sd_thread, sizeof(lsm_wa_sd_thread), NORMALPRIO +5, lsm_sd_thread, NULL);

    chSysLock();
    sd_slot_debounce_counter = 0;
    chVTSetI(&sd_slot_poll_timer, TIME_MS2I(POLLING_DELAY), poll_timer_handler, &SDCD1);
    chSysUnlock();

}
