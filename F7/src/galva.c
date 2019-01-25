//On Olimex E407 - D10 is for the output of X and D13 is for the output Y

#include <stdint.h>
#include "ch.h"
#include "hal.h"
#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"
#include "F7/galva.h"
#include "F7/control.h"

void lsm_galva_triangle_phase(void){
  SEGGER_RTT_printf(0, "Begin of a triangle signal on X and Y with T=%d ms\n", (2* (4092 / 4) * 1000 / 30000));
  while(1){
    for(uint16_t x=0 ; x < 4092 ; x+=4){
      control_scanner_xy(x,x);
    }
    for(uint16_t x=4092 ; x >0 ; x-=4){
      control_scanner_xy(x,x);
    }
  }
}

void lsm_galva_test(void){
  control_init(NORMALPRIO + 2);
  control_scanner_set_rate(30000);
  lsm_galva_triangle_phase();
}
