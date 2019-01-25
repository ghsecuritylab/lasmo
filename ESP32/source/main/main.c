#include "uart.h"
#include "wifi.h"
#include "nvs_flash.h"
#include "wifi.h"
#include <esp_http_server.h>


void app_main(){

  lsm_uart_esp_init();

  static httpd_handle_t server = NULL;
  ESP_ERROR_CHECK(nvs_flash_init());
  lsm_wifi_init(&server);

  lsm_uart_rxTask();
}
