#include <stdint.h>
#include "hal.h"
#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"
#include "F7_galva.h"

// This function must moove the galva to the x position
void lsm_control_galvaX(uint32_t x_to){
 if ( (x_to > 4095))
   SEGGER_RTT_printf(0, "Error, out of bounding ! \r\n");
 else{
   dacPutChannelX((DACDriver*) DAC1, 1U, x_to * 3.1/4095);
   SEGGER_RTT_printf(0, "The value %f is writing in the DAC Channel1\r\n", x_to * 3.1/4095);
 }
}

// This function must moove the galva to the y position
void lsm_control_galvaY(uint32_t y_to){
 if ( (y_to > 4095))
   SEGGER_RTT_printf(0, "Error, out of bounding ! \r\n");
 else{
   dacPutChannelX((DACDriver*) DAC1, 2U, y_to * 3.1/4095);
   SEGGER_RTT_printf(0, "The value %f is writing in the DAC Channel1\r\n", y_to * 3.1/4095);
 }
}

/*
 * Starting DAC1 driver, setting up the output pin as analog as suggested
 * by the Reference Manual.
 */
void lsm_galva_init(void){
  palSetPadMode(GPIOA, 4, PAL_MODE_INPUT_ANALOG);
}
