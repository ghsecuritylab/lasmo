#include "ch.h"
#include "hal.h"

#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"

#include "F7/lasers.h"
#include "F7/max5105.h"
#include "F7/control.h"

/* TEST CODES */
static THD_WORKING_AREA(lsm_test_thread_wa, 256);
static THD_FUNCTION(lsm_test_thread_fct, p) {
  (void)p;
  control_init(NORMALPRIO + 1);

  control_lasers_on();
  control_lasers_unmute();

 // while(1){
    SEGGER_RTT_printf(0, "##### TEST LASERs | RED - 1sec\n");
    control_lasers_set(0xFF,0x00,0x00);
    chThdSleepMilliseconds(1000);
/*
    SEGGER_RTT_printf(0, "##### TEST LASERs | GREEN - 1sec\n");
    control_lasers_set(0x00,0xFF,0x00);
    chThdSleepMilliseconds(1000);

    SEGGER_RTT_printf(0, "##### TEST LASERs | BLUE - 1sec\n");
    control_lasers_set(0x00,0x00,0xFF);
    chThdSleepMilliseconds(1000);

    SEGGER_RTT_printf(0, "##### TEST LASERs | WHITE - 1sec\n");
    control_lasers_set(0xFF,0xFF,0xFF);
    chThdSleepMilliseconds(1000);

    SEGGER_RTT_printf(0, "##### TEST LASERs | Sky blue - 1sec\n");
    control_lasers_set(0xAB,0xCD,0xEF);
    chThdSleepMilliseconds(1000);

    SEGGER_RTT_printf(0, "##### TEST LASERs | Yellow - 1sec\n");
    control_lasers_set(0xFF,0xC3,0x00);
    chThdSleepMilliseconds(1000); */
  //}
}

void lsm_lasers_test(void){
  chThdCreateStatic(lsm_test_thread_wa, sizeof(lsm_test_thread_wa),
                    NORMALPRIO + 1, lsm_test_thread_fct, NULL);
}

