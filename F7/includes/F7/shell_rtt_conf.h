#ifndef SHELL_RTT_CONF_H
#define SHELL_RTT_CONF_H

#include "F7/adc.h"
#include "F7/galva.h"
#include "F7/sd.h"

#define GETKEY_BUF_SIZE 255

char* shell_command_names[] = {
  "sd_test",
  "adc_test",
  "galva_test",
  NULL
};

void  (*shell_command_fcts[])(void) = {
  lsm_sd_test,
  lsm_adc_test,
  lsm_galva_test
};

#endif
