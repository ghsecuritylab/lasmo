#include <stdint.h>
#include "hal.h"
#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"
#include "F7_galva.h"

#define X_POSITION
#define Y_POSITION
#define DAC_BUFFER_SIZE 4096

static dacsample_t dac_buffer[DAC_BUFFER_SIZE];



static const DACConfig dac1cfg1 = {
  .init         = 0U,
  .datamode     = DAC_DHRM_12BIT_RIGHT,
  .cr           = 0
};

static void end_cb1(DACDriver *dacp, dacsample_t *buffer,size_t n) {

  (void)dacp;
  (void)buffer;

  if (dac_buffer == buffer) {
     dac_buffer[n];
  }
}

// This function must moove the galva to the x position
void lsm_control_galva(uint32_t x_to, uint32_t y_to){
 float x_to_analog ;
 if ( (x_to > 4095) || (y_to > 4095))
 SEGGER_RTT_printf(0, "Error, out of bounding ! \r\n");
 else{
  x_to_analog = x_to * 3.1/4095;
 }
}

/*
 * DAC error callback.
 */
static void error_cb1(DACDriver *dacp, dacerror_t err) {

  (void)dacp;
  (void)err;
  chSysHalt("DAC failure");
  SEGGER_RTT_printf(0, "DAC Failure ! \r\n");
}

static const DACConversionGroup dacgrpcfg1 = {
 .num_channels = 1U,
 .end_cb       = end_cb1,
 .error_cb     = error_cb1,
 .trigger      = DAC_TRG(0)
};

static const DACConversionGroup dacgrpcfg2 = {
 .num_channels = 2U,
 .end_cb       = end_cb1,
 .error_cb     = error_cb1,
 .trigger      = DAC_TRG(0)
};

/*
 * Starting DAC1 driver, setting up the output pin as analog as suggested
 * by the Reference Manual.
 */
void lsm_galva_init(void){
  palSetPadMode(GPIOA, 4, PAL_MODE_INPUT_ANALOG);
  dacStart(&DACD1, &dac1cfg1);
  for(int i = 0 ; i <4096; i++){
   dac_buffer[i] = i*3.1/4095;
  }

  dacStartConversion(&DACD1, &dacgrpcfg1,(dacsample_t *)dac_buffer, DAC_BUFFER_SIZE);
}
