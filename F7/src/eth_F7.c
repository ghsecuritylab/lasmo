#include "F7/eth_F7.h"

#include <string.h>

static uint8_t * current_buffer;
static struct netbuf * current_netbuf;
static uint16_t current_buflen;
static int offset;

static char boundary[75];

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
//static const char http_index_html[] = "<html><head><title>LASMO</title><body>Hello World</body></html>";
static const char http_index_html[] = "<!DOCTYPE html><html>\
  <head>\
    <meta charset = \"utf-8\">\
    <meta name = \"viewport\" content = \"width = device_widtch, initial-scale = 1\">\
    <title> 	Lasmo site </title>\
    <style type=\"text/css\">\
      body{\
        color:#444;\
        margin:40px auto;\
        max-width:650px;\
        line-height:1.6;\
        font-size:18px;\
        padding:0 10px\
      }\
      h1,h2,h3{line-height:1.2}\
    </style>\
  </head>\
  <body>\
    <text> <strong>	Bienvenue sur la page de LASMO </strong> <br><br>\
    <div>\
      <label for=\"file\">Choose a file to display</label>\
    </div>\
    <form enctype=\"multipart/form-data\" method=\"post\" action=\"/write\" required=\"true\" accept=\".ild|.ilda\"> <br>\
      <div>\
        Choose a file to upload:\
        <input name=\"uploaded_file\" type=\"file\"></input>\
        <input type=\"submit\" formmethod=\"post\"></input>\
      </div>\
    </form>\
    <form method=\"post\" action=\"/play\"><br><br>\
      <div>\
        <label>Play the animation:</label>\
        <input type=\"submit\" formmethod=\"post\"></input>\
      </div>\
    </form>\
  </body>\
</html>";

static void get_boundary(char* buf, char* dest) {
  char* boundary_buf = strstr(buf, "boundary=");
  if(boundary_buf == NULL) return;
  int blen = 0;
  for(blen = 0; boundary_buf[blen] != '\n'; blen++) ;
  dest[0] = '-';
  dest[1] = '-';
  strncpy(dest+2, boundary_buf+9, blen);
  dest[blen-6] = '\0';
  return;
}


static void http_post_handler(struct netconn * conn, char* buf, uint16_t buflen) {
  SEGGER_RTT_printf(0, "lsm_eth_thread: Info: POST request received!\n%s\n", buf);

  if(strstr(buf, "/write") != NULL) {
    get_boundary(buf, boundary);

    struct netbuf * inbuf;
    err_t err;
    while((err = netconn_recv(conn, &inbuf)) == ERR_OK) {
      netbuf_data(inbuf, (void**) &buf, &buflen);
      if(strstr(buf, boundary) != NULL) {
        break;
      }
      netbuf_delete(inbuf);
    }

    current_buffer = (uint8_t*)buf;
    current_buflen = buflen;
    current_netbuf = inbuf;
    offset = (int) (strstr(strstr(buf, boundary), "\r\n\r\n") - buf);
    offset += 4;

    lsm_sd_init();
    static FIL file;
    f_open(&file, "uploaded_file", FA_WRITE | FA_CREATE_ALWAYS);
    lsm_sd_write_file(&file, current_buffer + offset, current_buflen - offset);
    netbuf_delete(current_netbuf);

    while(netconn_recv(conn, &current_netbuf) == ERR_OK) {
      netbuf_data(current_netbuf, (void**) &current_buffer, &current_buflen);
      if(strstr((char*)current_buffer, boundary) != NULL){
        buflen = (int) (strstr((char*)current_buffer, boundary) - (char*)current_buffer);
        lsm_sd_write_file(&file, current_buffer, buflen);
        break;
      }
      lsm_sd_write_file(&file, current_buffer, current_buflen);
      netbuf_delete(current_netbuf);
    }

    f_close(&file);

    /* Send the HTML header
    * subtract 1 from the size, since we dont send the \0 in the string
    * NETCONN_NOCOPY: our data is const static, so no need to copy it
    */
    netconn_write(conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY);

    /* Send our HTML page */
    netconn_write(conn, http_index_html, sizeof(http_index_html)-1, NETCONN_NOCOPY);


  }

  else if(strstr(buf, "/play") != NULL) {
    lsm_sd_init();
    lsm_converter_init();
    lsm_converter_start("uploaded_file");

    /* Send the HTML header
    * subtract 1 from the size, since we dont send the \0 in the string
    * NETCONN_NOCOPY: our data is const static, so no need to copy it
    */
    netconn_write(conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY);

    /* Send our HTML page */
    netconn_write(conn, http_index_html, sizeof(http_index_html)-1, NETCONN_NOCOPY);
  }


  else if(strstr(buf, "/upload") == NULL) {
    netconn_write(conn, http_html_hdr, sizeof(http_html_hdr)-1, NETCONN_NOCOPY);
    netconn_write(conn, http_index_html, sizeof(http_index_html)-1, NETCONN_NOCOPY);
  } else {
    get_boundary(buf, boundary);

    SEGGER_RTT_printf(0, "Boundary: %s\n", boundary);
    // waiting for the beginning of the file
    struct netbuf * inbuf;
    err_t err;
    while((err = netconn_recv(conn, &inbuf)) == ERR_OK) {
      SEGGER_RTT_printf(0, "Getting a new packet buffer!\n");
      netbuf_data(inbuf, (void**)&buf, &buflen);
      SEGGER_RTT_printf(0, "Length: %d\n%s\n", buflen, (char*)buf);
      if(strstr(buf, boundary) != NULL) {
        SEGGER_RTT_printf(0, "Found boundary at %s, looks like: %s\n", strstr(buf, boundary), boundary);
        break;
      }

      netbuf_delete(inbuf);
    }

    SEGGER_RTT_printf(0, "Exited loop on error: %d\n", (int)err);

    current_buffer = (uint8_t*)buf;
    current_buflen = buflen;
    current_netbuf = inbuf;
    offset = (int) (strstr(strstr(buf, boundary), "\r\n\r\n") - buf);
    offset += 4;

    lsm_converter_init();
    lsm_converter_start_from_ethernet(conn);
  }

}

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
      netbuf_delete(inbuf);
    }

    else if(buflen>=6 &&
        buf[0]=='P' &&
        buf[1]=='O' &&
        buf[2]=='S' &&
        buf[3]=='T' &&
        buf[4]==' ' &&
        buf[5]=='/'
      ) {

      netbuf_delete(inbuf);

      //netconn_set_recvtimeout(conn, 5); //set netconn_recv timeout to 5 ms

      http_post_handler(conn,(char*) buf, buflen);

    }
  }


  /* Close the connection (server closes in HTTP) */
  netconn_close(conn);

  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
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
  SEGGER_RTT_printf(0, "\nS \r\n");
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
  conn->recv_timeout = 0;
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

static int boundary_reached = 0;

int lsm_eth_read_buffer(lsm_ilda_file_t * ilda_file, uint8_t* buffer, int buflen) {

  SEGGER_RTT_printf(0, "current_buflen: %d; offset: %d\n", current_buflen, offset);

  //first, check if the static buffer has enough data to fill the argument buffer
  //(if not, we have to retrieve data on the Ethernet port)
  if(offset + buflen > current_buflen){

    int already_copied = current_buflen - offset;
    //copy the remaining data of the static buffer in the argument buffer
    memcpy(buffer, current_buffer + offset, already_copied);
    offset = 0;

    // Go on to the next packet buffer in the netbuf, until the end of the netbuf or the filling of the argument buffer
    while(netbuf_next(current_netbuf) >= 0){
      netbuf_data(current_netbuf, (void**)&current_buffer, &current_buflen);

      // checking if the data in the static buffer is bigger than the one we have to fill
      if(current_buflen + already_copied >= buflen) {
        memcpy(buffer + already_copied, current_buffer, buflen - already_copied);
        offset = buflen - already_copied;
        // we can leave the function now, and return the number of bytes we were asked to read
        return buflen;
      }
      //if the data in the next packet buffer is still not enough, we copy it and go on to the next packet buffer
      // this is very unlikely in this project but necessary nonetheless
      else
        memcpy(buffer + already_copied, current_buffer, current_buflen);
    }

    // in case the data in the last netbuf was not enough to fill the arg buffer
    // we retrieve the next netbuf.
    netbuf_delete(current_netbuf);
    struct netbuf* inbuf;
    int err = netconn_recv(ilda_file->eth, &inbuf);
    if(err != ERR_OK) {
      SEGGER_RTT_printf(0, "lsm_eth_read: Info: No packet remaining\nError: %d\n", err);
      return 0;
    }

    SEGGER_RTT_printf(0, "lsm_eth_read: Info: Next netbuf!\nNetconn_recv error: %d\n", err);

    current_netbuf = inbuf;

    do {

      netbuf_data(current_netbuf, (void**)&current_buffer, &current_buflen);

      // check that we haven't reached the boundary
      if(strstr( (char*)current_buffer, boundary) != NULL && !boundary_reached) {
        int remaining_data = (int) (strstr((char*)current_buffer, boundary) - (char*) current_buffer);
        current_buflen = remaining_data;
        if(current_buflen < buflen)
          SEGGER_RTT_printf(0, "lsm_eth_read: Info: Could not fill the buffer cause the end of file has been reached\n");
        boundary_reached = 1;
      }

      // checking if the data in the static buffer is bigger than the one we have to fill
      if(current_buflen + already_copied > buflen) {
        memcpy(buffer + already_copied, current_buffer, buflen - already_copied);
        offset = buflen - already_copied;
        // we can leave the function now, and return the number of bytes we were asked to read
        return buflen;
      }
      //if the data in the next packet buffer is still not enough, we copy it and go on to the next packet buffer
      // this is very unlikely in this project but necessary nonetheless
      else {
        memcpy(buffer + already_copied, current_buffer, current_buflen);
      }

    } while(netbuf_next(current_netbuf) >= 0);

  } else {
    memcpy(buffer, current_buffer + offset, buflen);
    offset += buflen;
    return buflen;
  }

  return 0;
}
