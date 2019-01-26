#ifndef UART_H
#define UART_H


void lsm_uart_init(void);
void lsm_uart_test(void);
void lsm_uart_rx(void);
void lsm_uart_send_ilda_path(char array_path[][100]);

#endif
