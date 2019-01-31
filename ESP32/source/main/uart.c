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

#define ECHO_TEST_TXD  (GPIO_NUM_17)
#define ECHO_TEST_RXD  (GPIO_NUM_16)
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define BUF_SIZE (8000)

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

char files_path[6000];
static const char *RX_TASK_TAG = "RX_TASK";
static uint8_t data[BUF_SIZE+1];

volatile int good = 0;
static void lsm_esp_cp_tree(){
  int occur = 0;
  static int occur_path = 0;
  while(data [occur]){
    char * tmp = strstr((char*)data + occur, ".ild");
    if( tmp != NULL){
      size_t i = tmp - ((char*)data+occur) +4;
      strncpy((char*)(files_path + occur_path), (char*)(data+occur), i);
      occur += i ;
      occur_path += i ;
      files_path[occur_path]='\n';
      occur_path++;
    }
  }
  ESP_LOGI(RX_TASK_TAG, "DONE");
  good = 1;
}

static void lsm_esp_rx_task(){
  esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
  while (1) {
    const int rxBytes = uart_read_bytes(UART_NUM_1, data, BUF_SIZE, 1000 / portTICK_RATE_MS);
    if (rxBytes > 0) {
      data[rxBytes] = 0;
      lsm_esp_cp_tree();
    }
  }
}

void lsm_uart_rxTask(){
  xTaskCreate(lsm_esp_rx_task, "uart_rx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
}

void lsm_uart_txTask(){
  xTaskCreate(lsm_esp_tx_task, "uart_tx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
}
