#ifndef F7_LASERS_H
#define F7_LASERS_H

#include <stdint.h>

void lsm_lasers_init(void);

void lsm_lasers_mute(void);
void lsm_lasers_off(void);
void lsm_lasers_on(void);
void lsm_lasers_set(uint8_t r,uint8_t g, uint8_t b);
void lsm_lasers_unmute(void);

void lsm_lasers_test(void);

#endif

