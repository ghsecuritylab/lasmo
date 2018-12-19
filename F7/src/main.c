#include "ch.h"
#include "hal.h"
#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"
#include "F7/galva.h"

int main(void) {
  halInit();
  chSysInit();
  SEGGER_RTT_printf(0, "\r\n---------- Start ! ----------- \r\n");

  // RTT configuration
  SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);
  lsm_galva_init();

  lsm_ctrl_galvaXY(0,0);
  //Envoie de donnée ponctuelles
  while (1){
    chThdSleepMilliseconds(500);
    lsm_ctrl_galvaXY(1000,1000);
    chThdSleepMilliseconds(500);
    lsm_ctrl_galvaXY(2048,2048);
    chThdSleepMilliseconds(500);
    lsm_ctrl_galvaXY(4095,4095);
    chThdSleepMilliseconds(500);
   }
}
