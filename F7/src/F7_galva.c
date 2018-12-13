#include <stdint.h>
#include "hal.h"
#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"
#include "F7_galva.h"

#define X_POSITION
#define Y_POSITION
#define DAC_BUFFER_SIZE 4095
#define DAC_BUFFER_SIZE 4095

static const DACConfig dac1cfg1 = {
  .init         = 0U,
  .datamode     = DAC_DHRM_12BIT_RIGHT,
  .cr           = 0
};

static void end_cb1(DACDriver *dacp, dacsample_t *buffer) {

  (void)dacp;
  (void)buffer;

}

// This function must moove the galva to the x position
void lsm_control_galva(uint32_t x_to, uint32_t y_to){
 //echelle au max tourne a droite et achelle au min a gauche ,
 //4095 valeur pour aller de 0 à 3.1 V
 if ( (x_to > 4095) || (y_to > 4095))
 SEGGER_RTT_printf(0, "Error, out of bounding ! \r\n");
 else{
  //write in the dacs
  //  , 3,1
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

/*
 * Starting DAC1 driver, setting up the output pin as analog as suggested
 * by the Reference Manual.
 */
void lsm_galva_init(void){
  palSetPadMode(GPIOA, 4, PAL_MODE_INPUT_ANALOG);
  dacStart(&DACD1, &dac1cfg1);

//  dacStartConversion(&DACD1, &dacgrpcfg1,(dacsample_t *)lsm_control_galva, DAC_BUFFER_SIZE);
}
