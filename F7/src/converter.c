#include "ch.h"
#include "hal.h"
#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"

#include "F7/converter.h"
#include "F7/control.h"
#include "F7/decoder.h"

static size_t number_of_records;
static ilda_point_t frame_buffer[2][ILDA_BUFFER_SIZE];
static lsm_ilda_file_t myfile;
static char* curr_file_name;

// Pause functions
static MUTEX_DECL(pause_flag_mtx);
static uint8_t pause_flag = FALSE;
static uint8_t get_pause_flag(void){
  chMtxLock(&pause_flag_mtx);
  uint8_t r = pause_flag;
  chMtxUnlock(&pause_flag_mtx);
  return r;
}
static void set_pause_flag(uint8_t flag){
  chMtxLock(&pause_flag_mtx);
  pause_flag = flag;
  chMtxUnlock(&pause_flag_mtx);
}
void lsm_converter_pause(uint8_t state){
  set_pause_flag(state);
}

// Stop functions
static MUTEX_DECL(stop_flag_mtx);
static int stop_flag = TRUE;
static uint8_t get_stop_flag(void){
  chMtxLock(&stop_flag_mtx);
  uint8_t r = stop_flag;
  chMtxUnlock(&stop_flag_mtx);
  return r;
}
static void set_stop_flag(uint8_t flag){
  chMtxLock(&stop_flag_mtx);
  stop_flag = flag;
  chMtxUnlock(&stop_flag_mtx);
}

// wait_stoped
static BSEMAPHORE_DECL(display_stoped_bsem, 1);
void lsm_converter_wait_stoped(void){
  chBSemWait(&display_stoped_bsem);
}

//next frame functions
static BSEMAPHORE_DECL(next_frame_bsem, 0);
static THD_WORKING_AREA(frame_tim_thread_wa, 256);
static THD_FUNCTION(frame_tim_thread, p) {
  (void) p;
  while(1){
    chThdSleepMilliseconds(40);
    //send next_frame_bsem if we are not in pause or stoped
    if(!get_pause_flag() && !get_stop_flag()){
      chBSemSignal(&next_frame_bsem);
    }
  }
}

#define DISPLAY_END_WITH_NEXT_FRAME   0
#define DISPLAY_END_WITH_STOP         1
static int transform(int x) { return (x + 32768) * 4095 / 65535; }
//this function display one ilda_frame in loop until next frame or stop is called
static int display_frame(ilda_point_t* buf, size_t size_buf){
  (void) size_buf;
  while(1){
    for(size_t i=0; i < number_of_records ; ++i){
      if(ilda_is_blanking(buf[i].status_code)){
        control_lasers_mute();
      }
      else{
        control_lasers_set(buf[i].color.r,buf[i].color.g,buf[i].color.b);
        control_lasers_unmute();
      }
      control_scanner_xy(4095 - transform(buf[i].pos.x) , transform(buf[i].pos.y));

      //if stop is called
      if(get_stop_flag())
        return DISPLAY_END_WITH_STOP;
    }

    //if we have to change the frame
    switch (chBSemWaitTimeout(&next_frame_bsem,TIME_IMMEDIATE)) {
      case MSG_OK:
        number_of_records = lsm_decoder_switch_buffer();
        return DISPLAY_END_WITH_NEXT_FRAME;
      case MSG_TIMEOUT:
        continue;
      case MSG_RESET:
        control_emergency_halt("next frame semaphore was reset");
    }
  }
}

//this thread called display_frame for the current frame_buffer (unused by the decoder)
static BSEMAPHORE_DECL(display_start_bsem, 1);
static THD_WORKING_AREA(display_thread_wa, 256);
static THD_FUNCTION(display_thread, p) {
  (void) p;
  while(1){
    chBSemWait(&display_start_bsem);
    size_t i = 0;
    int display_end;
    do{
      display_end = display_frame(frame_buffer[i],ILDA_BUFFER_SIZE);
      i = (i+1) % 2;
    }
    while(display_end == DISPLAY_END_WITH_NEXT_FRAME);
  }
}

void lsm_converter_init(void){
  set_stop_flag(TRUE);

  lsm_sd_init();

  control_init(NORMALPRIO+2);
  control_scanner_set_rate(30000);

  //wait SD card
  while(!lsm_is_sd_connected()){
    chThdSleepMilliseconds(100);
  }

  lsm_decoder_init(frame_buffer);

  //create display thread
  chThdCreateStatic(display_thread_wa, sizeof(display_thread_wa), NORMALPRIO+1, display_thread, NULL);
  //create timer thread
  chThdCreateStatic(frame_tim_thread_wa, sizeof(frame_tim_thread_wa), NORMALPRIO+1, frame_tim_thread, 0);
}

void lsm_converter_start(char* file_name){
  if(!get_stop_flag())
    lsm_converter_stop();
  curr_file_name = file_name;
  lsm_sd_open_file(&myfile,file_name);
  lsm_decoder_start(&myfile);
  chThdSleepMilliseconds(500);
  number_of_records = lsm_decoder_switch_buffer();

  set_stop_flag(FALSE);
  chBSemSignal(&display_start_bsem);
}


void lsm_converter_stop(){
  set_stop_flag(TRUE);
  control_lasers_force_mute();
  lsm_decoder_stop();
  lsm_sd_close_file(&myfile);
  chBSemSignal(&display_stoped_bsem);

}

void lsm_converter_start_from_ethernet(struct netconn *conn) {

  myfile.eth = conn;
  myfile.flags = LSM_ILDA_FROM_ETHERNET;

  lsm_decoder_start(&myfile);

  chThdSleepMilliseconds(500);
  number_of_records = lsm_decoder_switch_buffer();

  /* control_lasers_unmute(); */

  set_stop_flag(FALSE);
  chBSemSignal(&display_start_bsem);
}

void lsm_converter_end_of_file(){
  set_stop_flag(TRUE);
  control_lasers_force_mute();
  lsm_sd_close_file(&myfile);
  chBSemSignal(&display_stoped_bsem);
}
