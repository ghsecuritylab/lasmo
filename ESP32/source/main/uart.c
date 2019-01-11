/* UART Select Example

This example code is in the Public Domain (or CC0 licensed, at your option.)

Unless required by applicable law or agreed to in writing, this
software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <sys/unistd.h>
#include <sys/select.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#define ECHO_TEST_TXD  (GPIO_NUM_15)
#define ECHO_TEST_RXD  (GPIO_NUM_16)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define BUF_SIZE (1024)

static const char* TAG = "uart";

void lsm_uart_esp_init(){
	uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity    = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE
	};
	uart_param_config(UART_NUM_1, &uart_config);
	uart_set_pin(UART_NUM_1, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
	uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);
		int fd;
	if ((fd = open("/dev/uart/0", O_RDWR)) == -1) {
		ESP_LOGE(TAG, "Cannot open UART\n");
		vTaskDelay(5000 / portTICK_PERIOD_MS);
	}
	ESP_LOGI(TAG, "Uart initiated");
}

int lsm_uart_esp_send_data(const char* name, const char * data){
	const int len = strlen(data);
	const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
	ESP_LOGI(name, "Wrote %d bytes", txBytes);
	return txBytes;
}

static void lsm_esp_tx_task(){
  static const char *TX_TASK_TAG = "TX_TASK";
  esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
  while (1) {
    lsm_uart_esp_send_data(TX_TASK_TAG,"Hello world!");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

static void lsm_esp_rx_task(){
  static const char *RX_TASK_TAG = "RX_TASK";
  esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
  uint8_t* data = (uint8_t*) malloc(BUF_SIZE+1);
  while (1) {
    const int rxBytes = uart_read_bytes(UART_NUM_1, data, BUF_SIZE, 1000 / portTICK_RATE_MS);
    if (rxBytes > 0) {
      data[rxBytes] = 3;
      ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
      ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
    }
  }
  free(data);
}

void lsm_uart_esp_test(){
	xTaskCreate(lsm_esp_rx_task, "uart_rx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
	xTaskCreate(lsm_esp_tx_task, "uart_tx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
}

void lsm_uart_txTask(){
		xTaskCreate(lsm_esp_tx_task, "uart_tx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
}
