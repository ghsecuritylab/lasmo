#include "ch.h"
#include "hal.h"
#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"
#include "F7/shell_rtt.h"

#include "F7/adc.h"
#include "F7/galva.h"
#include "F7/sd.h"
#include "F7/max5105.h"
#include "F7/lasers.h"
#include "F7/decoder.h"

static void lasers_sd_mute(int shutdown, int mute) {
  uint8_t state = MAX_DAC3_SHUTDOWN | MAX_DAC3_MUTE;
  if (shutdown) {
    state |= MAX_DAC0_SHUTDOWN | MAX_DAC1_SHUTDOWN | MAX_DAC2_SHUTDOWN;
  }
  if (mute) {
    state |= MAX_DAC0_MUTE | MAX_DAC1_MUTE | MAX_DAC2_MUTE;
  }
  lsm_max5105_wr_upd(MAX_SD_MUTE_ADDR, state);
}

int main(void) {
  halInit();
  chSysInit();
  SEGGER_RTT_Init();
  SEGGER_RTT_printf(0, "\r\n---------- Start ! ----------- \r\n");
  // RTT configuration
  SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);

  // lsm_shell_rtt_init();

  /* lsm_galva_init(); */
  /* lsm_galva_test(); */
  //lsm_sd_init();

  //lsm_adc_init();

  //lsm_max5105_init();
  //lsm_max5105_test();
  //lsm_adc_test();

  lsm_max5105_init();
  lasers_sd_mute(1, 1);
  lsm_lasers_test();

  //lsm_decoder_test("TheRiddle/theriddle.ild");

  while(1){
    chThdSleepMilliseconds(10);
    /* if(fs_ready == 0) */
      /* lsm_sd_test(); */
  }
}
