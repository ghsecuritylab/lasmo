/*
ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "F7/uart.h"
#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"
#include "F7/converter.h"
#include "F7/sd.h"
#include "F7/control.h"
#include <string.h>

#define SPEED   115200

void lsm_uart_send_ilda_path(char array_path[ILDA_PATH_MAX_SIZE]){
  size_t sz = strlen(array_path);
  msg_t msg = uartSendFullTimeout(&PORT_UART, &sz, array_path, 500);
  if (msg == MSG_OK){
  //  SEGGER_RTT_printf(0, "\r\nMessage Send \r\n");
  //  SEGGER_RTT_printf(0, "\nMessage content: %s\n", array_path);
  }
  else{
    SEGGER_RTT_printf(0, "\r\nInvalid Send Code Return \r\n");
  }
}

THD_WORKING_AREA(wa_uart_rcv_thrd, 2048);
THD_FUNCTION(uart_rcv, p);

/*
* UART driver configuration structure.
*/
static const UARTConfig uart_cfg = {
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  #ifndef BOARD_E407
  NULL,
  0,
  #endif
  SPEED,
  0,
  0,
  0
};

static void lsm_port_setup(void) {
  palSetPadMode(GPIO_UART, GPIO_UART_TX, PAL_MODE_ALTERNATE(8));
  palSetPadMode(GPIO_UART, GPIO_UART_RX, PAL_MODE_ALTERNATE(8));
}


void lsm_uart_init(void){
  lsm_port_setup();
  uartStart(&PORT_UART, &uart_cfg);
  SEGGER_RTT_printf(0, "\r\nUart initialisation \r\n");
}

THD_FUNCTION(uart_rcv, p){
  (void) p ;
  size_t sz_rcv;
  char size_rcv[3];
  sz_rcv = sizeof(size_rcv)-1;
  while(1){
    uartReceiveTimeout(&PORT_UART, &sz_rcv, size_rcv,TIME_INFINITE );
    size_rcv[2]='\0';
    //If we receive the stop message from the wifi
    if (strcmp(size_rcv, "st")== 0 ){
      lsm_converter_stop();
    }
    // If wwe receive the pause message from the wifi
    else if(strcmp(size_rcv,"ps")==0){
      lsm_converter_pause_swap();
    }
    // We set the rate at 12000 kpps
    else if(strcmp(size_rcv,"rd")==0){
      control_scanner_set_rate(12000);
    }
    //We set the rate at 30000 kpps
    else if(strcmp(size_rcv,"rt")==0){
      control_scanner_set_rate(30000);
    }
    else if(strcmp(size_rcv, "tr")==0){
      lsm_sd_send_tree();
    }
    else{
      int size_r = atoi(size_rcv);
      char path_rcv[size_r];
      size_t sz_path = sizeof(path_rcv);
      uartReceiveTimeout(&PORT_UART, &sz_path, path_rcv,TIME_INFINITE );
      path_rcv[sz_path]= '\0';
      lsm_converter_start(path_rcv);
    }
  }
}

void lsm_uart_rx(){
  chThdCreateStatic(wa_uart_rcv_thrd, sizeof(wa_uart_rcv_thrd),NORMALPRIO +1, uart_rcv, NULL);
}
