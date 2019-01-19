#include "ch.h"
#include "hal.h"
#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"
#include "ShellRTT/shell_rtt.h"

#include "F7/adc.h"
#include "F7/galva.h"
#include "F7/sd.h"
#include "F7/lasers.h"



int main(void) {
  halInit();
  chSysInit();
  SEGGER_RTT_Init();
  SEGGER_RTT_printf(0, "\r\n---------- Start ! ----------- \r\n");
  // RTT configuration
  SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);

  shell_rtt_init();

  /* lsm_galva_init(); */
  /* lsm_galva_test(); */

  //lsm_adc_init();

  //lsm_max5105_init();
  //lsm_max5105_test();
  //lsm_adc_test();

  //lsm_lasers_init();
  //lsm_lasers_test();

  while(1){
    chThdSleepMilliseconds(10);
    /* if(fs_ready == 0) */
      /* lsm_sd_test(); */
  }
}
