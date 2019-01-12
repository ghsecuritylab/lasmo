#include "ch.h"
#include "hal.h"

#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"

#include "F7/lasers.h"
#include "F7/max5105.h"

/* all muted, dac3 shutdown */
#define INIT_MUTE_SD_STATE ((~MAX_SHUTDOWN_ALL)|MAX_DAC3_SHUTDOWN|MAX_MUTE_ALL)
#define MUTE_RGB (MAX_DAC0_MUTE | MAX_DAC1_MUTE | MAX_DAC2_MUTE)

void lsm_lasers_init(void){
  /* init connection */
  lsm_max5105_init();

  lsm_lasers_on();
}

void lsm_lasers_on(void){
  /* init mute and shutdown states */
  lsm_max5105_wr_upd(MAX_SD_MUTE_ADDR,INIT_MUTE_SD_STATE);
}

void lsm_lasers_off(void){
  lsm_max5105_wr_upd(MAX_SD_MUTE_ADDR,MAX_SHUTDOWN_ALL | MAX_MUTE_ALL);
}

void lsm_lasers_set(uint8_t r,uint8_t g, uint8_t b){
  lsm_max5105_wr_upd(MAX_DAC0_ADDR ,r);
  lsm_max5105_wr_upd(MAX_DAC1_ADDR ,g);
  lsm_max5105_wr_upd(MAX_DAC2_ADDR ,b);
}

void lsm_lasers_mute(void){
  uint8_t shutdown_state = lsm_max5105_read(MAX_SD_MUTE_ADDR);
  lsm_max5105_wr_upd(MAX_SD_MUTE_ADDR,shutdown_state | MUTE_RGB);
}

void lsm_lasers_unmute(void){
  uint8_t shutdown_state = lsm_max5105_read(MAX_SD_MUTE_ADDR);
  lsm_max5105_wr_upd(MAX_SD_MUTE_ADDR, shutdown_state & ~MUTE_RGB);
}

static THD_WORKING_AREA(lsm_test_thread_wa, 256);
static THD_FUNCTION(lsm_test_thread_fct, p){
  (void)p;
  lsm_lasers_on();
  lsm_lasers_unmute();

  while(1){
    SEGGER_RTT_printf(0, "##### TEST LASERs | RED - 1sec\n");
    lsm_lasers_set(0xFF,0x00,0x00);
    chThdSleepMilliseconds(1000);

    SEGGER_RTT_printf(0, "##### TEST LASERs | GREEN - 1sec\n");
    lsm_lasers_set(0x00,0xFF,0x00);
    chThdSleepMilliseconds(1000);

    SEGGER_RTT_printf(0, "##### TEST LASERs | BLUE - 1sec\n");
    lsm_lasers_set(0x00,0x00,0xFF);
    chThdSleepMilliseconds(1000);

    SEGGER_RTT_printf(0, "##### TEST LASERs | WHITE - 1sec\n");
    lsm_lasers_set(0xFF,0xFF,0xFF);
    chThdSleepMilliseconds(1000);

    SEGGER_RTT_printf(0, "##### TEST LASERs | Sky blue - 1sec\n");
    lsm_lasers_set(0xAB,0xCD,0xEF);
    chThdSleepMilliseconds(1000);

    SEGGER_RTT_printf(0, "##### TEST LASERs | Yellow - 1sec\n");
    lsm_lasers_set(0xFF,0xC3,0x00);
    chThdSleepMilliseconds(1000);
  }
}

void lsm_lasers_test(void){
  chThdCreateStatic(lsm_test_thread_wa, sizeof(lsm_test_thread_wa),
                    NORMALPRIO + 1, lsm_test_thread_fct, NULL);
}

