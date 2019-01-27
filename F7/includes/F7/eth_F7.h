#ifndef F7_ETHERNET_H
#define F7_ETHERNET_H

#define WEB_THREAD_STACK_SIZE   1024
#define WEB_THREAD_PORT         80
#define WEB_THREAD_PRIORITY     (LOWPRIO + 2)

void lsm_dhcp_init(void);
void lsm_lwip_init_opt(void);
void lsm_ethernet_init(void);

#endif
