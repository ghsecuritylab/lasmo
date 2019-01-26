#ifndef F7_CONVERTER_H
#define F7_CONVERTER_H

#include <stdint.h>

void lsm_converter_init(void);
void lsm_converter_start(char* file_name);
void lsm_converter_stop(void);
void lsm_converter_pause(uint8_t state);

#endif
