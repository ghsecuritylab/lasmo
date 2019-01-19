#ifndef SD_H
#define SD_H

#include "ch.h"
#include "hal.h"
#include "ff.h"

#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"

// Will be removed when types.h is imported

#define LSM_ILDA_FROM_SD       1
#define LSM_ILDA_FROM_ETHERNET 2
#define LSM_ILDA_FROM_WIFI     3
#define LSM_ILDA_IS_SYNC       4

typedef struct ILDA_FILE {
  uint8_t flags;
  FIL orig_file;
} lsm_ilda_file;

// Availability of the filesystem
extern int fs_ready;

// Initiatilisation of the SD module
void lsm_sd_init(void);
void lsm_sd_test(void);

// Print directory
void lsm_sd_print_tree(void);

// File open/close functions for the decoder files (open files in read-only mode)
int lsm_sd_open_file(lsm_ilda_file* fp, char* path);
int lsm_sd_close_file(lsm_ilda_file* fp);

// Writes data to the specified file
UINT lsm_sd_write_file(FIL* fp, void* data, int datalen);

/* Read the content of a file (with or without an offset)
 *
 * Note: if no offset is specified, the read/write pointer will increase each
 *       time the function is called of the number of bytes read.
 *
 */
UINT lsm_sd_read_file(lsm_ilda_file* fp, char* buff, int buflen);
UINT lsm_sd_read_file_with_offset(lsm_ilda_file* fp, char* buff, int buflen, FSIZE_t offset);

// Event handler for insertion and removal of SD card
void lsm_sd_insert_handler(eventid_t id);
void lsm_sd_remove_handler(eventid_t id);

#endif
