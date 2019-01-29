#ifndef F7_ETHERNET_H
#define F7_ETHERNET_H

#define WEB_THREAD_STACK_SIZE   1024
#define WEB_THREAD_PORT         80
#define WEB_THREAD_PRIORITY     (LOWPRIO + 2)

#include "hal.h"
#include "ch.h"
#include "hal_mac_lld.h"
#include "lwipthread.h"
#include "lwip/dhcp.h"
#include "lwip/netif.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "F7/converter.h"
#include "F7/types.h"
#include "sd.h"

//#include "multipart-parser-c/multipart_parser.h"

void lsm_dhcp_init(void);
void lsm_lwip_init_opt(void);
void lsm_ethernet_init(void);

int lsm_eth_read_buffer(lsm_ilda_file_t* opaque, uint8_t* buffer, int buflen);

#endif
