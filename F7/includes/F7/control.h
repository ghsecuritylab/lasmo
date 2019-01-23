#ifndef F7_CONTROL_H
#define F7_CONTROL_H

#include "ch.h"
#include <stdint.h>

void control_init(tprio_t prio);

__attribute__((noreturn))
void control_emergency_halt(const char *reason);

void control_lasers_on(void);
void control_lasers_off(void);
void control_lasers_mute(void);
void control_lasers_unmute(void);
void control_lasers_set(uint8_t r, uint8_t g, uint8_t b);

void control_scanner_xy(uint16_t x, uint16_t y);

#endif // F7_CONTROL_H
