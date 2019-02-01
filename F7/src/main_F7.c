#include "ch.h"
#include "hal.h"

#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"

#include "F7/shell_rtt.h"
#include "F7/adc.h"
#include "F7/galva.h"
#include "F7/sd.h"
#include "F7/lasers.h"
#include "F7/decoder.h"
#include "F7/uart.h"
#include "F7/converter.h"
#include "F7/eth_F7.h"

int main(void) {
  halInit();
  chSysInit();

  // RTT configuration
  SEGGER_RTT_Init();
  SEGGER_RTT_ConfigUpBuffer(0, NULL, NULL, 0, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
  SEGGER_RTT_printf(0, "\r\n---------- Start ! ----------- \r\n");

  lsm_converter_init();
  lsm_converter_start("ILDA_files/geometry/geometry.ild");

  //lsm_uart_init();
  //lsm_ethernet_init();

  chThdSleep(TIME_INFINITE);
}
