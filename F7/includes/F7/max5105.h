#ifndef F7_MAX5105_H
#define F7_MAX5105_H

#include <stdint.h>

// addrs
#define MAX_DAC0_ADDR         0x00
#define MAX_DAC1_ADDR         0x04
#define MAX_DAC2_ADDR         0x08
#define MAX_DAC3_ADDR         0x0C
#define MAX_SD_MUTE_ADDR      0x10

//values
#define MAX_DAC0_MUTE         0x10
#define MAX_DAC1_MUTE         0x20
#define MAX_DAC2_MUTE         0x40
#define MAX_DAC3_MUTE         0x80
#define MAX_MUTE_ALL          0xF0

void lsm_max5105_init(void);

void    lsm_max5105_load  (uint8_t addr);
uint8_t lsm_max5105_read  (uint8_t addr);
void    lsm_max5105_wr_upd(uint8_t addr, uint8_t data);
void    lsm_max5105_write (uint8_t addr, uint8_t data);
#ifndef NO_HARDWARE_MUTE
// Hardware mute. Can be called from any context.
void    lsm_max5105_hw_muteX(uint8_t mute);
#endif // !NO_HARDWARE_MUTE

void lsm_max5105_test(void);

#endif

