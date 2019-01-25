/*  Inspired by WiFi softAP Example*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include <esp_http_server.h>
#include <sys/param.h>

#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "uart.h"
#define ESP_WIFI_SSID      "lasmo"
#define ESP_WIFI_PASS      "lasmo123"

static const char *TAG="wifi";

extern const uint8_t lasmo_html_str[] asm("_binary_lasmo_html_start");
extern const uint8_t underscore_str[] asm("_binary_underscore_js_start");
extern const uint8_t paths_str[] asm("_binary_paths_txt_start");

/* An HTTP GET handler */
esp_err_t hello_get_handler(httpd_req_t *req) {
  ESP_LOGI(TAG, "GET handler");
  httpd_resp_send(req, (const char*) lasmo_html_str, strlen((const char*) lasmo_html_str));
  return ESP_OK;
}

httpd_uri_t index_uri = {
  .uri       = "/lasmo.html",
  .method    = HTTP_GET,
  .handler   = hello_get_handler,
};

/* An HTTP GET handler */
esp_err_t underscore_get_handler(httpd_req_t *req){
  ESP_LOGI(TAG, "GET handler");
  httpd_resp_send(req, (const char*) underscore_str, strlen((const char*) underscore_str));
  return ESP_OK;
}

httpd_uri_t underscore_uri = {
  .uri       = "/underscore.js",
  .method    = HTTP_GET,
  .handler   = underscore_get_handler,
};

/* An HTTP GET handler */
esp_err_t path_get_handler(httpd_req_t *req) {
  ESP_LOGI(TAG, "GET handler");
  httpd_resp_send(req, (const char*) paths_str, strlen((const char*) paths_str));
  return ESP_OK;
}

httpd_uri_t path_uri = {
  .uri       = "/paths.txt",
  .method    = HTTP_GET,
  .handler   = path_get_handler,
};

/* An HTTP POST handler */

esp_err_t response_post_handler(httpd_req_t *req){
  ESP_LOGI(TAG, "POST handler");
  int buffer_size = 128;
  char buffer[buffer_size];
  static char char_to_send[128];
  char int_buf [2];

  if (httpd_req_recv(req, buffer, req->content_len) < 0) {
    ESP_LOGI(TAG, "Problem receiving POST");
    return ESP_FAIL;
  }
  ESP_LOGI(TAG, "Value = %s", buffer);
  char * tmp = strstr(buffer, ".ild");
  tmp[4]='\0';
  if (tmp != NULL){
    size_t i = tmp - buffer +5;
    strncpy((char*)(char_to_send), (char*)(buffer), i);
    sprintf(int_buf, "%d", i-1);
    lsm_uart_esp_send_data(TAG,int_buf);
    ESP_LOGI(TAG, "Char_send= %s",char_to_send);
    lsm_uart_esp_send_data(TAG, char_to_send);
  }
  httpd_resp_send(req, (const char*) lasmo_html_str, strlen((const char*) lasmo_html_str));
  buffer[0]= 0;
  return ESP_OK;
}

httpd_uri_t response_uri = {
  .uri       = "/response",
  .method    = HTTP_POST,
  .handler   = response_post_handler,
  .user_ctx  = NULL
};

httpd_handle_t start_webserver(void){
  httpd_handle_t server = NULL;
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();

  // Start the httpd server
  ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
  if (httpd_start(&server, &config) == ESP_OK) {
    // Set URI handlers
    ESP_LOGI(TAG, "Registering URI handlers");
    httpd_register_uri_handler(server, &index_uri);
    httpd_register_uri_handler(server, &underscore_uri);
    httpd_register_uri_handler(server, &path_uri);
    httpd_register_uri_handler(server, &response_uri);
    return server;
  }
  ESP_LOGI(TAG, "Error starting server!");
  return NULL;
}

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
  httpd_handle_t *server = (httpd_handle_t *) ctx;

  switch(event->event_id) {
    case SYSTEM_EVENT_AP_STACONNECTED:
    ESP_LOGI(TAG, "station:"MACSTR" join, AID=%d",
    MAC2STR(event->event_info.sta_connected.mac),
    event->event_info.sta_connected.aid);
    break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
    ESP_LOGI(TAG, "station:"MACSTR"leave, AID=%d",
    MAC2STR(event->event_info.sta_disconnected.mac),
    event->event_info.sta_disconnected.aid);
    break;
    case SYSTEM_EVENT_AP_START:
    ESP_LOGI(TAG, "SYSTEM_EVENT_AP_START");
    // Start the web server
    if (*server == NULL) {
      *server = start_webserver();
    }
    break;
    default:
    break;
  }
  return ESP_OK;
}

void lsm_wifi_init(void *arg) {
  tcpip_adapter_init();
  ESP_ERROR_CHECK(esp_event_loop_init(event_handler, arg));

  tcpip_adapter_ip_info_t ip_info;
  IP4_ADDR(&(ip_info.ip), 192, 168, 0, 1);
  IP4_ADDR(&(ip_info.netmask), 255, 255, 255, 0);
  IP4_ADDR(&(ip_info.gw), 192, 168, 0, 1);
  ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP));
  ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &ip_info));
  ESP_ERROR_CHECK(tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP));

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  wifi_config_t wifi_config = {
    .ap = {
      .ssid = ESP_WIFI_SSID,
      .password = ESP_WIFI_PASS,
      .ssid_len = strlen(ESP_WIFI_SSID),
      .authmode = WIFI_AUTH_WPA_WPA2_PSK,
      .max_connection = 10,
    },
  };
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(TAG, "Wifi initiated");
}
