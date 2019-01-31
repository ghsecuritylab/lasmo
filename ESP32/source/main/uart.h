#ifndef UART_H
#define UART_H

void lsm_uart_esp_init(void);
int lsm_uart_esp_send_data(const char* name, const char * data);
void lsm_uart_txTask(void);
void lsm_uart_rxTask(void);

extern char files_path[6000];

#endif
