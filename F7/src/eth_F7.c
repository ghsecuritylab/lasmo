#include "hal.h"

#include "ch.h"
#include "hal_mac_lld.h"
#include "lwipthread.h"
#include "lwip/dhcp.h"
#include "lwip/netif.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "F7/eth_F7.h"


// MAC addresse configuration
static uint8_t mac_add[6] = {0xe4, 0x42, 0xa6 , 0x8f, 0x65, 0xe3};

// Specification of MAC's options and IP address.
// Here, IP, network and gateway are initialized at 0
// Specification of DHCP
lwipthread_opts_t opt = {
  mac_add,
  0,
  0,
  0,
  (net_addr_mode_t) NET_ADDRESS_DHCP
};

void lsm_lwip_init_opt(void){
  chThdSleepMilliseconds(500);
  lwipInit(&opt);
  SEGGER_RTT_printf(0, "Lwip init done. \r\n");
  chThdSleepMilliseconds(500);
}

void lsm_dhcp_init(void){
  static int i = 0;
  // The ip address is specified by the function dhcp_start in the netif_default
  // This function is reloaded until the ipadress is setted
  while(!(dhcp_supplied_address(netif_default))){
    // Control if the ip adress is setted each 1 sec, and reloaded dhcp_start
    if((i%10)==9){
      dhcp_stop(netif_default);
      dhcp_start(netif_default);
    }
    SEGGER_RTT_printf(0, "DHCP loaded %d times \r\n", i);
    i++;
    chThdSleepMilliseconds(1000);
  }
  SEGGER_RTT_printf(0, "Succeed, an ip address was found !! \r\n");

  SEGGER_RTT_printf(0, "IP given: %s", ip4addr_ntoa(&(netif_default->ip_addr)));
}

static const char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
static const char http_index_html[] = "<html><head><title>LASMO</title></head><body>Hello Word!</body></html>";

static void http_server_serve(struct netconn *conn) {
  struct netbuf *inbuf;
  char *buf;
  u16_t buflen;
  err_t err;

  /* Read the data from the port, blocking if nothing yet there.
   We assume the request (the part we care about) is in one netbuf */
  err = netconn_recv(conn, &inbuf);

  if (err == ERR_OK) {
    netbuf_data(inbuf, (void **)&buf, &buflen);

    /* Is this an HTTP GET command? (only check the first 5 chars, since
    there are other formats for GET, and we're keeping it very simple )*/
    if (buflen>=5 &&
        buf[0]=='G' &&
        buf[1]=='E' &&
        buf[2]=='T' &&
        buf[3]==' ' &&
        buf[4]=='/' ) {

      /* Send the HTML header
             * subtract 1 from the size, since we dont send the \0 in the string
             * NETCONN_NOCOPY: our data is const static, so no need to copy it
       */
      netconn_write(conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY);

      /* Send our HTML page */
      netconn_write(conn, http_index_html, sizeof(http_index_html)-1, NETCONN_NOCOPY);
    }
  }
  SEGGER_RTT_printf(0, "Succeed!! \r\n");
  /* Close the connection (server closes in HTTP) */
  netconn_close(conn);
  SEGGER_RTT_printf(0, "Succeed 1 \r\n");

  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
  netbuf_delete(inbuf);
}


/**
 * Stack area for the http thread.
 */
static THD_WORKING_AREA(wa_http_server, WEB_THREAD_STACK_SIZE);

/**
 * HTTP server thread.
 */
static THD_FUNCTION(http_server, p) {
  struct netconn *conn = NULL;
  struct netconn *newconn = NULL;
  err_t err;

  (void)p;
  chRegSetThreadName("http");
  SEGGER_RTT_printf(0, "S \r\n");
  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);
  LWIP_ERROR("http_server: invalid conn", (conn != NULL), chThdExit(MSG_RESET););

  /* Bind to port 80 (HTTP) with default IP address */
  netconn_bind(conn, NULL, WEB_THREAD_PORT);
  SEGGER_RTT_printf(0, "U \r\n");

  /* Put the connection into LISTEN state */
  netconn_listen(conn);
  SEGGER_RTT_printf(0, "C \r\n");
  /* Goes to the final priority after initialization.*/
  chThdSetPriority(WEB_THREAD_PRIORITY);
  SEGGER_RTT_printf(0, "E \r\n");
  while (true) {
    err = netconn_accept(conn, &newconn);
    if (err != ERR_OK)
      continue;
    http_server_serve(newconn);
    netconn_delete(newconn);
  }

}

void lsm_ethernet_init(void){
  lsm_lwip_init_opt();
  lsm_dhcp_init();

  chThdCreateStatic(wa_http_server, sizeof(wa_http_server), WEB_THREAD_PRIORITY, http_server, NULL);

}
