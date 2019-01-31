/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Simon Goldschmidt
 *
 */
#ifndef LWIP_HDR_LWIPOPTS_H__
#define LWIP_HDR_LWIPOPTS_H__

/* Fixed settings mandated by the ChibiOS integration.*/
#include "static_lwipopts.h"

#define LWIP_DHCP                      TRUE
#define LWIP_DEBUG                     FALSE
#define LWIP_UDP                       TRUE

// Option débug pour DHCP
/*
#define DHCP_DEBUG                     LWIP_DBG_ON
#define NETIF_DEBUG                     LWIP_DBG_ON
#define API_LIB_DEBUG                   LWIP_DBG_ON
#define API_MSG_DEBUG                   LWIP_DBG_ON
#define IP_DEBUG                        LWIP_DBG_ON
#define IP_REASS_DEBUG                  LWIP_DBG_ON
#define UDP_DEBUG                       LWIP_DBG_ON
#define ICMP_DEBUG                      LWIP_DBG_ON
#define RAW_DEBUG                       LWIP_DBG_ON
*/

//#define ETHARP_DEBUG                    LWIP_DBG_ON
//#define PBUF_DEBUG                      LWIP_DBG_ON
//#define SOCKETS_DEBUG                   LWIP_DBG_ON
//#define INET_DEBUG                      LWIP_DBG_ON
//#define MEM_DEBUG                       LWIP_DBG_ON
//#define MEMP_DEBUG                      LWIP_DBG_ON
//#define SYS_DEBUG                       LWIP_DBG_ON
//#define TIMERS_DEBUG                    LWIP_DBG_ON
//#define TCP_DEBUG                       LWIP_DBG_ON
//#define TCPIP_DEBUG                     LWIP_DBG_ON
//#define DNS_DEBUG                      LWIP_DBG_ON

#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"

/* Optional, application-specific settings.*/
#if !defined(TCPIP_MBOX_SIZE)
#define TCPIP_MBOX_SIZE                 MEMP_NUM_PBUF
#endif

#if !defined(TCPIP_THREAD_STACKSIZE)
#define TCPIP_THREAD_STACKSIZE          1024
#endif

#define TCP_MSS 1460
#define TCP_WND 7300

#define DEFAULT_TCP_RECVMBOX_SIZE       6
#define DEFAULT_ACCEPTMBOX_SIZE         4

#define LWIP_SO_RCVTIMEO 1

/* Use ChibiOS specific priorities. */
#if !defined(TCPIP_THREAD_PRIO)
#define TCPIP_THREAD_PRIO               (LOWPRIO + 1)
#endif
#if !defined(LWIP_THREAD_PRIORITY)
#define LWIP_THREAD_PRIORITY            (LOWPRIO)
#endif

#define LWIP_PLATFORM_DIAG(msg)    dbg_function msg

#define LWIP_DNS                        TRUE
#define LWIP_DNS_SECURE                 0

//MAC Address byte 0.
#define LWIP_ETHADDR_0                      0xe3

// MAC Address byte 1.
#define LWIP_ETHADDR_1                      0x65
//  MAC Address byte 2.
#define LWIP_ETHADDR_2                      0x8f

//MAC Address byte 3.
#define LWIP_ETHADDR_3                      0xa6

//MAC Address byte 4.
#define LWIP_ETHADDR_4                      0x42

//MAC Address byte 5.
#define LWIP_ETHADDR_5                      0xe4

static inline int dbg_function(const char * sFormat, ...) {
   int r;
   va_list ParamList;

   va_start(ParamList, sFormat);
   r = SEGGER_RTT_vprintf(0, sFormat, &ParamList);
   va_end(ParamList);
   return r;
}
#endif /* LWIP_HDR_LWIPOPTS_H__ */
