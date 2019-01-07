#ifndef SD_H
#define SD_H

#include "ch.h"
#include "hal.h"
#include "ff.h"

#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"

// Availability of the filesystem
extern int fs_ready;

// Initiatilisation of the SD module
void lsm_sd_init(void);
void lsm_sd_test(void);

// Print directory
void lsm_print_tree(void);

/* Read the content of a file (with or without an offset)
 *
 * Note: if no offset is specified, the read/write pointer will increase each
 *       time the function is called of the number of bytes read.
 *
 */
UINT lsm_sd_read_file(FIL* fp, char* buff, int buflen);
UINT lsm_sd_read_file_with_offset(FIL* fp, char* buff, int buflen, FSIZE_t offset);

// Event handler for insertion and removal of SD card
void lsm_insert_handler(eventid_t id);
void lsm_remove_handler(eventid_t id);

#endif
