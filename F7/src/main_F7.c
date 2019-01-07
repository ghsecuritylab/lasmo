#include "ch.h"
#include "hal.h"
#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"

#include "F7/sd.h"

int main(void) {
 halInit();
 chSysInit();
 SEGGER_RTT_printf(0, "Start ! \r\n");

 // RTT configuration
 SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_BLOCK_IF_FIFO_FULL);

 lsm_sd_init();

 while(1){
  chThdSleepMilliseconds(10);
  if(fs_ready == 0)
    lsm_sd_test();
 }
}
