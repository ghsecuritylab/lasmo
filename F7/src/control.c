#include "hal.h"
#include "F7/control.h"
#include "F7/max5105.h"

/*
 * CONSTANTS DEFINING BEHAVIOUR OF THIS MODULE
 */

#define MAX_SAFE_MOVE_INTERVAL_MS 25
#define MAX_RATE_PPS 30000

/*
 * LASERS CONTROL
 */

static void lasers_mute(int mute) {
  lsm_max5105_wr_upd(MAX_SD_MUTE_ADDR,
      mute ? MAX_DAC0_MUTE | MAX_DAC2_MUTE | MAX_DAC3_MUTE : 0);
}

static void lasers_init(void) {
  lsm_max5105_init();
#ifdef NO_HARDWARE_MUTE
  lasers_mute(1);
#else
  lasers_mute(0);
#endif // NO_HARDWARE_MUTE
}

/*
 * SCANNER CONTROL
 */

#define SCANNER_CHANNEL1 0U
#define SCANNER_CHANNEL2 1U

static const DACConfig dac1cfg1 = {
  .init         = 0U,
  .datamode     = DAC_DHRM_12BIT_RIGHT_DUAL,
  .cr           = 0
};

static void scanner_xy(int16_t x_value, uint16_t y_value) {
  dacPutChannelX(&DACD1, SCANNER_CHANNEL1, x_value / 2);
  dacPutChannelX(&DACD1, SCANNER_CHANNEL2, y_value / 2);
}

/*
 * Starting DAC1 driver, setting up the output pin as analog as suggested
 * by the Reference Manual.
 */
static void scanner_init(void) {
  palSetPadMode(DAC1_GPIO, DAC1_PIN, PAL_MODE_INPUT_ANALOG);
  palSetPadMode(DAC2_GPIO, DAC2_PIN, PAL_MODE_INPUT_ANALOG);
  dacObjectInit(&DACD1);
  dacStart(&DACD1, &dac1cfg1);

  /* Prevent the scanner from moving several times in succession at startup */
  chThdSleepMilliseconds(1);
}

/*
 * INTEGRATED COMMAND CONTROL
 */

static BSEMAPHORE_DECL(next_frame_ok, 0);

static void next_frameISR(GPTDriver *gptp) {
  (void) gptp;
  chSysLockFromISR();
  chBSemSignalI(&next_frame_ok);
  chSysUnlockFromISR();
}

#define RATE_DIVISOR_PPS 1200000

static const GPTConfig timer_cfg = {
  RATE_DIVISOR_PPS,
  next_frameISR,
  0,
  0
};

static void next_frame_timer_init(void) {
  gptStart(&GPTD5, &timer_cfg);
}

#define MIN_RATE_PPS (1000 / MAX_SAFE_MOVE_INTERVAL_MS)
#define MAX_SAFE_MOVE_INTERVAL_TICKS (TIME_MS2I(MAX_SAFE_MOVE_INTERVAL_MS))

#define COMMAND_SCANNER_MOVE 0
#define COMMAND_LASERS_ON 1
#define COMMAND_LASERS_OFF 2
#define COMMAND_LASERS_MUTE 3
#define COMMAND_LASERS_UNMUTE 4
#define COMMAND_LASERS_SET 5
#define COMMAND_SCANNER_RATE 6

#define MAX_SCANNER_VALUE 4095

#define CONTROL_MBOX_SIZE 1
static msg_t control_mbox_data[CONTROL_MBOX_SIZE];
static MAILBOX_DECL(control_mbox, control_mbox_data, CONTROL_MBOX_SIZE);

static THD_WORKING_AREA(control_thread_wa, 256);

static THD_FUNCTION(control_thread, p) {
  (void) p;
  next_frame_timer_init();
  scanner_init();
#ifndef BOARD_E407
  lasers_init();
#endif
  int is_muted = 1;
  systime_t last_move = 0;
  uint16_t last_x = 0;
  uint16_t last_y = 0;
  uint16_t rate_divisor = RATE_DIVISOR_PPS / 30000 - 1;
  uint8_t last_r = 0;
  uint8_t last_g = 0;
  uint8_t last_b = 0;
  for (;;) {
    const sysinterval_t since_last_move = chVTGetSystemTimeX() - last_move;
    if (!is_muted && since_last_move >= MAX_SAFE_MOVE_INTERVAL_TICKS) {
      control_emergency_halt("no move in max allowed interval while laser is on");
    }
    msg_t command;
    switch (chMBFetchTimeout(&control_mbox,
          &command,
          (!is_muted)
            ? MAX_SAFE_MOVE_INTERVAL_TICKS - since_last_move
            : TIME_INFINITE)) {
      case MSG_OK:
        {
          const uint32_t data = command & 0xfffffff;
          switch ((command >> 28) & 0xf) {
            case COMMAND_LASERS_MUTE:
              is_muted = 1;
              break;
            case COMMAND_LASERS_UNMUTE:
              is_muted = 0;
              break;
            case COMMAND_LASERS_SET:
              {
                uint8_t r = data >> 16;
                uint8_t g = data >> 8;
                uint8_t b = data;
                if (r != last_r || g != last_g || b != last_b) {
                  lsm_max5105_wr_upd(MAX_DAC0_ADDR, r);
                  lsm_max5105_wr_upd(MAX_DAC2_ADDR, g);
                  lsm_max5105_wr_upd(MAX_DAC3_ADDR, b);
                  last_r = r;
                  last_g = g;
                  last_b = b;
                }
              }
              break;
            case COMMAND_SCANNER_MOVE:
              {
                // Wait until we have the right to move, but not above
                // the safety delay in case the timer does not fire for
                // any reason. In this case, we abort.
                switch (chBSemWaitTimeout(&next_frame_ok, MAX_SAFE_MOVE_INTERVAL_TICKS)) {
                  case MSG_OK:
                    gptStartOneShot(&GPTD5, rate_divisor);
                    break;
                  case MSG_TIMEOUT:
                    control_emergency_halt("next frame timer did not fire in time");
                  case MSG_RESET:
                    control_emergency_halt("next frame semaphore was reset");
                }
                const uint16_t x = data >> 16;
                const uint16_t y = data & 0xffff;
                if (x > MAX_SCANNER_VALUE || y > MAX_SCANNER_VALUE) {
                  control_emergency_halt("coordinates out of bound");
                }
                if (x != last_x || y != last_y || is_muted) {
                  last_move = chVTGetSystemTimeX();
                  last_x = x;
                  last_y = y;
                }
#ifdef NO_HARDWARE_MUTE
                lasers_mute(is_muted);
#else
                lsm_max5105_hw_muteX(is_muted);
#endif // NO_HARDWARE_MUTE
                scanner_xy(x, y);
                break;
              }
            case COMMAND_SCANNER_RATE:
              if (data < MIN_RATE_PPS || data > MAX_RATE_PPS)
                control_emergency_halt("scanner rate out of bounds");
              rate_divisor = RATE_DIVISOR_PPS / data - 1;
              break;
            default:
              control_emergency_halt("unknown command received");
          }
          break;
        }
      case MSG_RESET:
        control_emergency_halt("control queue has been reset");
      case MSG_TIMEOUT:
        // Will be handled at the next occurrence.
        {}
    }
  }
}

static void send_command(uint8_t command, msg_t data) {
  if (data & 0xf0000000) {
    control_emergency_halt("malformed command");
  }
  msg_t msg = (command << 28) | data;
  chMBPostTimeout(&control_mbox, msg, TIME_INFINITE);
}

/*
 * USER-VISIBLE FUNCTIONS
 */

static int initialized = 0;

void control_emergency_halt(const char *reason) {
#ifndef NO_HARDWARE_MUTE
  lsm_max5105_hw_muteX(1);
#endif // !NO_HARDWARE_MUTE
  lasers_mute(1);
  chSysHalt(reason);
  for (;;);
}

void control_init(tprio_t prio) {
  if (initialized) {
    control_emergency_halt("control system already initialized");
  } else {
    chThdCreateStatic(control_thread_wa, sizeof(control_thread_wa), prio,
        control_thread, 0);
    initialized = 1;
  }
}

void control_lasers_mute(void) {
  send_command(COMMAND_LASERS_MUTE, 0);
}

void control_lasers_force_mute(void) {
  control_lasers_mute();
  control_scanner_xy(0, 0);
}

void control_lasers_unmute(void) {
  send_command(COMMAND_LASERS_UNMUTE, 0);
}

void control_lasers_set(uint8_t r, uint8_t g, uint8_t b) {
#ifdef MONOCHROME_MODE
#ifdef ATTENUATE_MODE
#error MONOCHROME_MODE and ATTENUATE_MODE are exclusive
#endif // ATTENUATE_MODE
  // 4.9V * 171 / 255 = 3.29V
  r = r || b || g ? 171 : 0;
  g = 0;
  b = 0;
#endif // MONOCHROME_MODE
#ifdef ATTENUATE_MODE
  r = r * 125 / 490;
  g = g * 125 / 490;
  b = b * 125 / 490;
#endif // ATTENUATE_MODE
  send_command(COMMAND_LASERS_SET, (r << 16) | (g << 8) | b);
}

void control_scanner_xy(uint16_t x, uint16_t y) {
  send_command(COMMAND_SCANNER_MOVE, (x << 16) | y);
}

void control_scanner_set_rate(uint16_t pps) {
  send_command(COMMAND_SCANNER_RATE, pps);
}

void control_write_eeprom_configuration() {
  lasers_init();
  lsm_max5105_write(MAX_SD_MUTE_ADDR, MAX_MUTE_ALL);
  lsm_max5105_write(MAX_DAC0_ADDR, 0);
  lsm_max5105_write(MAX_DAC1_ADDR, 0);
  lsm_max5105_write(MAX_DAC2_ADDR, 0);
  lsm_max5105_write(MAX_DAC3_ADDR, 0);
  chThdSleepMilliseconds(15);
}
