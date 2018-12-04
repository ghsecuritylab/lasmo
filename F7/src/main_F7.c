#include "ch.h"
#include "hal.h"
#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"

int main(void) {
 halInit();
 chSysInit();
 SEGGER_RTT_printf(0, "Start ! \r\n");

 // RTT configuration
 SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);

 while(1){
  chThdSleepMilliseconds(10);
  SEGGER_RTT_printf(0, "Test ! \r\n");
 }
}
