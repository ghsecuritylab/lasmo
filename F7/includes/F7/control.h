#ifndef F7_CONTROL_H
#define F7_CONTROL_H

#include "ch.h"
#include <stdint.h>

// The following symbols may be defined while compiling this module:
// - NO_HARDWARE_MUTE: mute using the DAC control rather than the
//                     dedicated output
// - MONOCHROME_MODE:  use only the DAC0 channel in 3.3V output all or
//                     nothing (instead of 0-4.9V).

// Initialize control module. The priority must be high enough so that
// the lasers can be safely muted if required. Lasers start muted
// at a 30kPPS speed.
void control_init(tprio_t prio);

// Mute the lasers and turn the system off. This should be called in case
// of emergency.
__attribute__((noreturn))
void control_emergency_halt(const char *reason);

// Mute the lasers.
// The lasers must be unmuted prior to this operation.
void control_lasers_mute(void);

// Unmute the lasers.
// The lasers must be muted prior to this operation.
void control_lasers_unmute(void);

// Set the lasers color, without impacting their visibility.
// In monochrome mode, any non-zero value will turn the laser on.
void control_lasers_set(uint8_t r, uint8_t g, uint8_t b);

// Set the scanner position. x and y must both be in the [0; 4096[
// range. The scanner will not move at a faster rate than what has
// been allowed.
void control_scanner_xy(uint16_t x, uint16_t y);

// Set the scanner rate in pulse per second.
void control_scanner_set_rate(uint16_t pps);

// Write the permanent configuration into the non-volatile DAC
// registers: lasers on, muted, value at 0.
void control_write_eeprom_configuration(void);

#endif // F7_CONTROL_H
