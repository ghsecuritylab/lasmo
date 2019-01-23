#ifndef SD_H
#define SD_H

#include "ch.h"
#include "hal.h"
#include "ff.h"

#include "F7/types.h"

#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"

// Initiatilisation of the SD module
void lsm_sd_init(void);
void lsm_sd_test(void);

// Print directory
void lsm_sd_print_tree(void);

// File open/close functions for the decoder files (open files in read-only mode)
int lsm_sd_open_file(lsm_ilda_file_t* fp, const char* path);
int lsm_sd_close_file(lsm_ilda_file_t* fp);

// Writes data to the specified file
UINT lsm_sd_write_file(FIL* fp, void* data, int datalen);

// Writes data to the specified file
UINT lsm_sd_write_file(FIL* fp, void* data, int datalen);

/* Read the content of a file (with or without an offset)
 *
 * Note: if no offset is specified, the read/write pointer will increase each
 *       time the function is called of the number of bytes read.
 *
 */
int lsm_sd_read_file(lsm_ilda_file_t* fp, char* buff, int buflen);
int lsm_sd_read_file_with_offset(lsm_ilda_file_t* fp, char* buff, int buflen, FSIZE_t offset);

// Event handler for insertion and removal of SD card
void lsm_sd_insert_handler(eventid_t id);
void lsm_sd_remove_handler(eventid_t id);

int lsm_is_sd_connected(void);

#endif
