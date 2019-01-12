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

  //Envoie de donnée ponctuelles
  lsm_ctrl_galvaXY(0,0);
  lsm_ctrl_galvaXY(2048,2048);
  lsm_ctrl_galvaXY(4095,4095);

  lsm_buf_ctrl_galva();
  while(1){
   chThdSleepMilliseconds(10);
  }
}
