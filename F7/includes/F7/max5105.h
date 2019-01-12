#ifndef F7_MAX5105_H
#define F7_MAX5105_H

#include <stdint.h>

#define MAX_DAC0_ADDR    0x00
#define MAX_DAC1_ADDR    0x01
#define MAX_DAC2_ADDR    0x02
#define MAX_DAC3_ADDR    0x03
#define MAX_SD_MUTE_ADDR 0x04

#define MAX_DAC0_SHUTDOWN    0x01
#define MAX_DAC1_SHUTDOWN    0x02
#define MAX_DAC2_SHUTDOWN    0x04
#define MAX_DAC3_SHUTDOWN    0x08
#define MAX_SHUTDOWN_ALL     0x0F
#define MAX_DAC0_MUTE        0x10
#define MAX_DAC1_MUTE        0x20
#define MAX_DAC2_MUTE        0x40
#define MAX_DAC3_MUTE        0x80
#define MAX_MUTE_ALL         0xF0

void lsm_max5105_init(void);
void lsm_max5105_write(uint8_t addr, uint8_t data);
void lsm_max5105_wr_upd(uint8_t addr, uint8_t data);
void lsm_max5105_load(uint8_t addr);
uint8_t lsm_max5105_read(uint8_t addr);
void lsm_max5105_test(void);

#endif

