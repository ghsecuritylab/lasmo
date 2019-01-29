#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include "ff.h"
#include "lwipthread.h"
#include "lwip/dhcp.h"
#include "lwip/netif.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"

#define LSM_ILDA_FROM_SD       0x01
#define LSM_ILDA_FROM_ETHERNET 0x02
#define LSM_ILDA_FROM_WIFI     0x03
#define LSM_ILDA_IS_SYNC       0x04
#define LSM_ILDA_IS_OLD_FORMAT 0x08

#define ILDA_BUFFER_SIZE       3000

// Top-level structure modeling an ILDA file. COntains info about the file origin and if it's synced or not.
typedef struct {
  // the two last bits determine the ILDA file's origin (SD (01), Ethernet(10), WiFi(11))
  // bit 3 determine if the file is synced with the jack input (1) or not
  // bit 4 determine if the file is to be read in strict mode (1) or not
  uint8_t flags;
  // if in SD mode, the pointer to the file on the SD card
  FIL orig_file;
  // if in Ethernet mode, the pointer to the object representing the connection
  struct netconn* eth;
} lsm_ilda_file_t;


typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} lsm_color_t;

typedef struct {
  uint16_t x;
  uint16_t y;
  uint16_t z;
  lsm_color_t c;
} lsm_point_t;

#endif
