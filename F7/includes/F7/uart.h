#ifndef UART_H
#define UART_H

#define ILDA_PATH_MAX_SIZE 100


void lsm_uart_init(void);
void lsm_uart_tx(void);
void lsm_uart_rx(void);
void lsm_uart_send_ilda_path(char array_path[ILDA_PATH_MAX_SIZE]);

#endif
