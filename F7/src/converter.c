#include "ch.h"
#include "hal.h"
#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"

#include "F7/types.h"

#include "F7/converter.h"
#include "F7/control.h"
#include "F7/decoder.h"

static size_t number_of_records;
static uint8_t pause_flag = 0;

static BSEMAPHORE_DECL(next_frame, 0);
static THD_WORKING_AREA(frame_tim_thread_wa, 256);
static THD_FUNCTION(frame_tim_thread, p) {
  (void) p;
  while(1){
    chThdSleepMilliseconds(40);
    if(!pause_flag){
      chBSemSignal(&next_frame);
    }
  }
}

//static void lsm_converter_pause(uint8_t state){
//  pause_flag = state;
//}

static int transform(int x) { return (x + 32768) * 4095 / 65535; }

static void display_frame(ilda_point_t* buf, size_t size_buf){
  (void) size_buf;
  size_t i=0;
  while(1){
    control_scanner_xy(transform(buf[i].pos.x) , transform(buf[i].pos.y));
    ++i;
    i %= number_of_records;
    //set laser here

    //test if we have to quit
    switch (chBSemWaitTimeout(&next_frame,TIME_IMMEDIATE)) {
      case MSG_OK:
        number_of_records = lsm_decoder_switch_buffer();
        return;
      case MSG_TIMEOUT:
        continue;
      case MSG_RESET:
        control_emergency_halt("next frame semaphore was reset");
    }
  }
}

static ilda_point_t frame_buffer[2][ILDA_BUFFER_SIZE];
static lsm_ilda_file_t myfile;

void lsm_converter_init(char* file_name){
  lsm_sd_init();
  while(!lsm_is_sd_connected()){
    chThdSleepMilliseconds(100);
  }

  /* lsm_sd_open_file(&myfile,"ILDA_files/tunnel/tunnelride2.ild"); */
  lsm_sd_open_file(&myfile,file_name);
  lsm_decoder_decode(frame_buffer,&myfile);

  chThdSleepMilliseconds(500);
  number_of_records = lsm_decoder_switch_buffer();
  /* size_t size_bufs = fill_bufs_circle(); */

  control_init(NORMALPRIO+2);
  control_scanner_set_rate(12000);

  /* control_lasers_on(); */
  /* control_lasers_unmute(); */
  chThdCreateStatic(frame_tim_thread_wa, sizeof(frame_tim_thread_wa), NORMALPRIO+2, frame_tim_thread, 0);
}

void lsm_converter_start(void){
  for(;;){
    display_frame(frame_buffer[1],ILDA_BUFFER_SIZE);
    display_frame(frame_buffer[0],ILDA_BUFFER_SIZE);
  }
}

