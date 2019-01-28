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
#include <string.h>

#define SPEED   115200

static char* file_name[] = {
  "ILDA_files/earth/earth.ild",
  "ILDA_files/plane/1plane.ild",
  "ILDA_files/riddle2/theriddle.ild",
  "ILDA_files/pacman/PL.ild",
  "ILDA_files/romain/cirlce.ild",
  NULL
};

void lsm_choice(char value ){
  SEGGER_RTT_printf(0,"Reading file : %s\n", file_name[(size_t)value-49]);
  if (value-49 == -1)
    lsm_converter_stop();
  else{
    lsm_converter_start(file_name[(size_t)value-49]);
  }
}
void lsm_uart_send_ilda_path(char array_path[ILDA_PATH_MAX_SIZE]){
  size_t sz = strlen(array_path);
  msg_t msg = uartSendFullTimeout(&PORT_UART, &sz, array_path, 500);
  if (msg == MSG_OK){
    SEGGER_RTT_printf(0, "\r\nMessage Send \r\n");
    SEGGER_RTT_printf(0, "\nMessage content: %s\n", array_path);
  }
  else{
    SEGGER_RTT_printf(0, "\r\nInvalid Send Code Return \r\n");
  }
}


THD_WORKING_AREA(wa_uart_send_thrd, 128);
THD_FUNCTION(uart_send, data);

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
  lsm_uart_rx();
  lsm_sd_send_tree_to_esp();
}

THD_FUNCTION(uart_rcv, p){
  (void) p ;
  msg_t msg ;
  size_t sz_rcv;
  char size_rcv[2];
  sz_rcv = sizeof(size_rcv);
  while(1){
    msg = uartReceiveTimeout(&PORT_UART, &sz_rcv, size_rcv,TIME_INFINITE );
    if (msg == MSG_OK){
      SEGGER_RTT_printf(0, "\r\nSize Receive :\n");
      for (unsigned int i = 0; i <sz_rcv; i++){
        SEGGER_RTT_printf(0, "%c", size_rcv[i]);
      }
      SEGGER_RTT_printf(0, "\n");
    }
    else
      SEGGER_RTT_printf(0, "\r\nInvalid Received Code Return \r\n");

    int size_r = atoi(size_rcv);
    char path_rcv[size_r];
    size_t sz_path = sizeof(path_rcv);
    msg = uartReceiveTimeout(&PORT_UART, &sz_path, path_rcv,TIME_INFINITE );
    path_rcv[sz_path]= '\0';

    if (msg == MSG_OK){
      SEGGER_RTT_printf(0, "\r\nBuffer full:\n");
      for (unsigned int i = 0; i <sz_path; i++){
        SEGGER_RTT_printf(0, "%c", path_rcv[i]);
      }
      SEGGER_RTT_printf(0, "\n");
    }
    else
      SEGGER_RTT_printf(0, "\r\nInvalid Received Code Return \r\n");
    lsm_converter_start(path_rcv);
  }
}

THD_FUNCTION(uart_send, data){
  (void) data ;
  msg_t msg ;
  size_t sz ;
  static const char tx_buf[] = "0123456789ABCDEF";
  int compteur = 0;
  while(1){
    sz = sizeof(tx_buf);
    msg = uartSendFullTimeout(&PORT_UART, &sz, tx_buf, 500);
    if (msg == MSG_OK){
      SEGGER_RTT_printf(0, "\r\nMessage %d Send \r\n", compteur);
      compteur++;
    }
    else
      SEGGER_RTT_printf(0, "\r\nInvalid Send Code Return \r\n");
    chThdSleepMilliseconds(1000);
  }
}

void lsm_uart_rx(){
  chThdCreateStatic(wa_uart_rcv_thrd, sizeof(wa_uart_rcv_thrd),NORMALPRIO +1, uart_rcv, NULL);
}

void lsm_uart_tx(){
  chThdCreateStatic(wa_uart_send_thrd, sizeof(wa_uart_send_thrd),NORMALPRIO + 1, uart_send, NULL);
}
