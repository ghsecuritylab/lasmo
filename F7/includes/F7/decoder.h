#ifndef DECODER_H
#define DECODER_H

#include <stdint.h>
#include <string.h>

#include "F7/sd.h"
//#include "F7/wifi.h"
//#include "F7/ethernet.h"

//#include "F7/converter.h"
#include "ILDA_decoder/ilda_decoder.h"
#include "F7/types.h"

// Makes the decoder switch buffer once he has finished filling up one
void lsm_decoder_switch_buffer(void);

// Contains the index of the buffer the decoder is currently filling
extern int current_working_buffer_index;

// Launches a thread that decodes the ILDA file passed as argument 2,
// using the two first buffers of the buffer array passed as argument 1.
// The thread pauses when a buffer is filled. You must then use lsm_decoder_switch_buffer()
// to make the thread continue, and fill the other buffer.
// Returns a pointer to the thread so that you can use chThdWait() to wait for the thread
thread_t* lsm_decoder_decode(ilda_point_t** frame_buffer, lsm_ilda_file_t* ilda_file);

// Test function, decodes the specified ILDA file located on the SD card
void lsm_decoder_test(const char* filepath);

#endif
