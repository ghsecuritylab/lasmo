#include "F7/decoder.h"

int current_working_buffer_index;

static int frame_read_done;
static BSEMAPHORE_DECL(next_frame, 0);
static lsm_ilda_file_t * working_ilda_file;
static ilda_point_t* frame_buffers[2];

static size_t number_of_records;
size_t lsm_decoder_switch_buffer(void) {
  current_working_buffer_index = (current_working_buffer_index + 1) % 2;
  chBSemSignal(&next_frame);
  return number_of_records;
}

static ssize_t read_from_sd_card(void* opaque, void* buffer, size_t length){
  return (ssize_t) lsm_sd_read_file((lsm_ilda_file_t*) opaque, buffer, length);
}

static ssize_t read_from_wifi(void* opaque, void* buffer, size_t length){
  //TODO: complete when WiFi API is ready
  (void)opaque;
  (void)buffer;
  (void)length;
  return 0;
}

static ssize_t read_from_ethernet(void* opaque, void* buffer, size_t length){
  //TODO: complete when Ethernet API is ready
  (void)opaque;
  (void)buffer;
  (void)length;
  return 0;
}

static THD_WORKING_AREA(lsm_wa_decoder_thread, 1024);

// Main thread for the decoder. Takes the ILDA file to decode in argument
static THD_FUNCTION(lsm_decoder_thread, arg){
  (void) arg;

  int reading_mode = working_ilda_file->flags & 0x03;
  int strict_mode  = working_ilda_file->flags & 0x08;

  static ilda_state_t ilda;

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
      chThdExit(1);
  }

  SEGGER_RTT_printf(0, "lsm_decoder_thread: Info: Starting reading file...\n\n");

  for(;;){
    const ilda_header_t *current_header = ilda_read_next_header(&ilda);
    if(!(current_header)){
      SEGGER_RTT_printf(0, "lsm_decoder_thread: Error: Invalid header: %s\n", ilda.error);
      chThdExit(1);
    }

    if(ilda_is_end_of_file(current_header)) {
      SEGGER_RTT_printf(0, "lsm_decoder_thread: Info: Reached end of file!");
      break;
    }

    if(ilda_is_palette(current_header)){
      if(ilda_read_palette(&ilda)){
        SEGGER_RTT_printf(0, "lsm_decoder_thread: Error: Invalid palette: %s\n", ilda.error);
        chThdExit(1);
      }
    }

    number_of_records = current_header->number_of_records;

    if(ilda_read_records(
          &ilda,
          frame_buffers[current_working_buffer_index],
          ILDA_BUFFER_SIZE*sizeof(ilda_point_t)
       )){
      SEGGER_RTT_printf(0, "lsm_decoder_thread: Error: %s\n", ilda.error);
      chThdExit(1);
    }

    frame_read_done = 1;

    chBSemWaitTimeout(&next_frame,TIME_INFINITE);
  }
  chThdExit(0);
}

thread_t* lsm_decoder_decode(ilda_point_t myframe_buffer[2][ILDA_BUFFER_SIZE], lsm_ilda_file_t* ilda_file) {

  working_ilda_file = ilda_file;

  frame_buffers[0] = myframe_buffer[0];
  frame_buffers[1] = myframe_buffer[1];
  return chThdCreateStatic(lsm_wa_decoder_thread, sizeof(lsm_wa_decoder_thread), NORMALPRIO +1, lsm_decoder_thread, NULL);
}

