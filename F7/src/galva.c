//On Olimex E407 - D10 is for the output of X and D13 is for the output Y

#include <stdint.h>
#include "ch.h"
#include "hal.h"
#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"
#include "F7/galva.h"

#define CHANNEL1  0U
#define CHANNEL2  1U

#define MAX_VALUE 4095
/* 30kpps -> T=33,33us */
#define MIN_PTS_PERIOD_US 34

static systime_t last_pts;
static const DACConfig dac1cfg1 = {
  .init         = 0U,
  .datamode     = DAC_DHRM_12BIT_RIGHT_DUAL,
  .cr           = 0
};

void lsm_ctrl_galvaXY(int16_t x_value, uint16_t y_value){
  if( (x_value > MAX_VALUE) | (y_value > MAX_VALUE) )
    SEGGER_RTT_printf(0, "Error, XY out of bounding !\n");
  else if( TIME_I2US(chVTTimeElapsedSinceX(last_pts)) < MIN_PTS_PERIOD_US )
    SEGGER_RTT_printf(0, "Error, XY frequency too high !\n");
  else{
    dacAcquireBus(&DACD1 );
    dacPutChannelX(&DACD1, CHANNEL1, x_value);
    dacPutChannelX(&DACD1, CHANNEL2, y_value);
    dacReleaseBus(&DACD1 );
    last_pts = chVTGetSystemTime();
  }
}

/*
 * Starting DAC1 driver, setting up the output pin as analog as suggested
 * by the Reference Manual.
 */
void lsm_galva_init(void){
  palSetPadMode(DAC1_GPIO, DAC1_PIN, PAL_MODE_INPUT_ANALOG);
  palSetPadMode(DAC2_GPIO, DAC2_PIN, PAL_MODE_INPUT_ANALOG);
  dacObjectInit(&DACD1);
  dacStart(&DACD1, &dac1cfg1);

  last_pts = chVTGetSystemTime();

  chThdSleepMicroseconds(1);
  systime_t tmp = chVTTimeElapsedSinceX(last_pts);
  SEGGER_RTT_printf(0, "Minimum time measurment: %dus\n", TIME_I2US(tmp));

  /* for no conflic on first point */
  chThdSleepMicroseconds(MIN_PTS_PERIOD_US);
}

void lsm_galva_triangle_phase(void){
  uint32_t t_us = 500;
  SEGGER_RTT_printf(0, "Begin of a triangle signal on X and Y with T=%d ms\n", (t_us * 2* (4092 / 4) / 1000));
  while(1){
    for(uint16_t x=0 ; x < 4092 ; x+=4){
      lsm_ctrl_galvaXY(x,x);
      chThdSleepMicroseconds(t_us);
    }
    for(uint16_t x=4092 ; x >0 ; x-=4){
      lsm_ctrl_galvaXY(x,x);
      chThdSleepMicroseconds(t_us);
    }
  }
}

void lsm_galva_test(void){
  lsm_galva_init();
  lsm_galva_triangle_phase();
}

