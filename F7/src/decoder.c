#include "F7/decoder.h"
#include "F7/converter.h"

int current_working_buffer_index;

static int frame_read_done;
static BSEMAPHORE_DECL(next_ilda_file, 1);
static lsm_ilda_file_t * working_ilda_file;
static ilda_point_t* frame_buffers[2];

#define EVT_STOP        0
#define EVT_NEXT_FRAME  1
event_source_t evt_play;

// Stop functions
void lsm_decoder_stop(void) {
  chEvtBroadcastFlags(&evt_play,EVT_STOP);
}

static size_t number_of_records;
size_t lsm_decoder_switch_buffer(void) {
  current_working_buffer_index = (current_working_buffer_index + 1) % 2;
  chEvtBroadcastFlags(&evt_play,EVT_NEXT_FRAME);
  return number_of_records;
}

static ssize_t read_from_sd_card(void* opaque, void* buffer, size_t length){
  ssize_t n = lsm_sd_read_file((lsm_ilda_file_t*) opaque, buffer, length);
  if (n == 0) {
    f_lseek(&((lsm_ilda_file_t*) opaque)->orig_file, 0); 
    n = lsm_sd_read_file((lsm_ilda_file_t*) opaque, buffer, length);
  }
  return n;
}

static ssize_t read_from_wifi(void* opaque, void* buffer, size_t length){
  //TODO: complete when WiFi API is ready
  (void)opaque;
  (void)buffer;
  (void)length;
  return 0;
}

static ssize_t read_from_ethernet(void* opaque, void* buffer, size_t length){
  return (ssize_t) lsm_eth_read_buffer((lsm_ilda_file_t*) opaque, (uint8_t*)buffer, length);
}

static THD_WORKING_AREA(lsm_wa_decoder_thread, 1024);

// Main thread for the decoder. Takes the ILDA file to decode in argument
static THD_FUNCTION(lsm_decoder_thread, arg){
  (void) arg;
  event_listener_t evt_listener;
  chEvtRegister(&evt_play,&evt_listener,0);
  static ilda_state_t ilda;

  int reading_mode;
  int strict_mode;
stop_label:
  chBSemWait(&next_ilda_file);

  reading_mode = working_ilda_file->flags & 0x03;
  strict_mode  = working_ilda_file->flags & 0x08;

  switch (reading_mode) {
    case LSM_ILDA_FROM_SD:
      ilda_init(&ilda, read_from_sd_card, (void*) working_ilda_file, strict_mode);
      break;
    case LSM_ILDA_FROM_WIFI:
      ilda_init(&ilda, read_from_wifi, (void*) working_ilda_file, strict_mode);
      break;
    case LSM_ILDA_FROM_ETHERNET:
      ilda_init(&ilda, read_from_ethernet, (void*) working_ilda_file, strict_mode);
      break;
    default:
      SEGGER_RTT_WriteString(0, "lsm_decoder_thread: Error: Invalid reading mode\n");
      lsm_converter_end_of_file();
      goto stop_label;
  }

  SEGGER_RTT_printf(0, "lsm_decoder_thread: Info: Starting reading file...\n\n");

  SEGGER_RTT_printf(0, "Frame number\tFormat Code\tFrame Name\tCompany name\tNb of records\tTotal frames\n\n");

  for(;;){
    const ilda_header_t *current_header = ilda_read_next_header(&ilda);

    if(!(current_header)){
      SEGGER_RTT_printf(0, "lsm_decoder_thread: Error: Invalid header: %s\n", ilda.error);
      lsm_converter_end_of_file();
      goto stop_label;
    }

    if(ilda_is_end_of_file(current_header)) {
      SEGGER_RTT_printf(0, "lsm_decoder_thread: Info: Reached end of file, looping!");
      continue;
    }

    if(ilda_is_palette(current_header)){
      if(ilda_read_palette(&ilda)){
        SEGGER_RTT_printf(0, "lsm_decoder_thread: Error: Invalid palette: %s\n", ilda.error);
        lsm_converter_end_of_file();
        goto stop_label;
      }
    }

    number_of_records = current_header->number_of_records;

    if(ilda_read_records(
          &ilda,
          frame_buffers[current_working_buffer_index],
          ILDA_BUFFER_SIZE*sizeof(ilda_point_t)
       )){
      SEGGER_RTT_printf(0, "lsm_decoder_thread: Error: %s\n", ilda.error);
      lsm_converter_end_of_file();
      goto stop_label;
    }

    frame_read_done = 1;

    chEvtWaitAny(EVENT_MASK(0));
    uint8_t evt = chEvtGetAndClearFlags(&evt_listener);
    switch(evt){
      case EVT_STOP:
        goto stop_label;
      case EVT_NEXT_FRAME:
        break;
    }
  }
  chThdExit(0);
}

static thread_t* decoder_thread;
void lsm_decoder_init(ilda_point_t myframe_buffer[2][ILDA_BUFFER_SIZE]) {
  chEvtObjectInit(&evt_play);

  frame_buffers[0] = myframe_buffer[0];
  frame_buffers[1] = myframe_buffer[1];
  decoder_thread = chThdCreateStatic(lsm_wa_decoder_thread, sizeof(lsm_wa_decoder_thread), NORMALPRIO +1, lsm_decoder_thread, NULL);
}

void lsm_decoder_start(lsm_ilda_file_t* ilda_file) {
  working_ilda_file = ilda_file;
  chBSemSignal(&next_ilda_file);
}
