#ifndef F7_CONTROL_H
#define F7_CONTROL_H

#include "ch.h"
#include <stdint.h>

// Initialize control module. The priority must be high enough so that
// the lasers can be safely turned off if required. Lasers start off
// and muted, at a 30kPPS speed.
void control_init(tprio_t prio);

// Stop the lasers and turn the system off. This should be called in case
// of emergency.
__attribute__((noreturn))
void control_emergency_halt(const char *reason);

// Turn the lasers on without impacting their muted state.
// The lasers must be off prior to this operation.
// Scanner must move fast enough as long as the lasers are
// on (even if muted), otherwise this module will stop the
// lasers and the system.
void control_lasers_on(void);

// Turn the lasers off without impacting their muted state.
// The lasers must be on prior to this operation.
void control_lasers_off(void);

// Mute the lasers without impacting their on/off state.
// The lasers must be unmuted prior to this operation.
void control_lasers_mute(void);

// Unmute the lasers without impacting their on/off state.
// The lasers must be muted prior to this operation.
void control_lasers_unmute(void);

// Set the lasers color, without impacting their visibility.
void control_lasers_set(uint8_t r, uint8_t g, uint8_t b);

// Set the scanner position. x and y must both be in the [0; 4096[
// range. The scanner will not move at a faster rate than what has
// been allowed.
void control_scanner_xy(uint16_t x, uint16_t y);

// Set the scanner rate in pulse per second.
void control_scanner_set_rate(uint16_t pps);

// Write the permanent configuration into the non-volatile DAC
// registers: lasers off, muted, value at 0.
void control_write_eeprom_configuration(void);

#endif // F7_CONTROL_H
