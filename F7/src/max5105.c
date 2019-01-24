#include "ch.h"
#include "hal.h"

#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"

#include "F7/max5105.h"

#ifndef NO_HARDWARE_MUTE
#define MUTE_LINE PAL_LINE(GPIOD, 7)
#endif // NO_HARDWARE_MUTE

/* Serial Interface Programming Commands */
#define MAX_WRITE   0x20
#define MAX_WR_UPD  0x28
#define MAX_READ    0x30
#define MAX_LOAD    0x38

#define SPI_BUFFERS_SIZE    2
static uint8_t txbuf[SPI_BUFFERS_SIZE];
static uint8_t rxbuf[SPI_BUFFERS_SIZE];

static const SPIConfig hs_spicfg = {
  false,
  NULL,
  SPI1_NSS_GPIO,
  SPI1_NSS_PIN,
  SPI_CR1_BR_2 | SPI_CR1_BR_1,
  /* SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0 */
  0x700
};

static void lsm_max5105_swap_bufs(void){
  spiAcquireBus(&SPID1);
  spiStart(&SPID1, &hs_spicfg);

  spiSelect(&SPID1);
  spiExchange(&SPID1, SPI_BUFFERS_SIZE, txbuf, rxbuf);
  spiUnselect(&SPID1);

  spiReleaseBus(&SPID1);
}

void lsm_max5105_init(void){
  palSetPadMode(SPI1_NSS_GPIO , SPI1_NSS_PIN , PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(SPI1_MOSI_GPIO, SPI1_MOSI_PIN, PAL_MODE_ALTERNATE(5)   );
  palSetPadMode(SPI1_MISO_GPIO, SPI1_MISO_PIN, PAL_MODE_ALTERNATE(5)   );
  palSetPadMode(SPI1_SCK_GPIO , SPI1_SCK_PIN , PAL_MODE_ALTERNATE(5)   );

  /* for safety,all dac are muted and shutdowned */
#ifdef NO_HARDWARE_MUTE
  lsm_max5105_wr_upd(MAX_SD_MUTE_ADDR, MAX_SHUTDOWN_ALL | MAX_MUTE_ALL);
#else
  palSetLineMode(MUTE_LINE, PAL_MODE_OUTPUT_PUSHPULL);
  lsm_max5105_hw_muteX(1);
  lsm_max5105_wr_upd(MAX_SD_MUTE_ADDR, MAX_SHUTDOWN_ALL);
#endif // NO_HARDWARE_MUTE
}


void lsm_max5105_load(uint8_t addr){
    txbuf[0] = MAX_LOAD | addr;
    lsm_max5105_swap_bufs();
}

uint8_t lsm_max5105_read(uint8_t addr){
    txbuf[0] = MAX_READ | addr;
    lsm_max5105_swap_bufs();
    return rxbuf[1];
}

void lsm_max5105_wr_upd(uint8_t addr, uint8_t data){
    txbuf[0] = MAX_WR_UPD | addr;
    txbuf[1] = data;
    lsm_max5105_swap_bufs();
}

void lsm_max5105_write(uint8_t addr, uint8_t data){
    txbuf[0] = MAX_WRITE | addr;
    txbuf[1] = data;
    lsm_max5105_swap_bufs();
}

/* TEST CODE */
static THD_WORKING_AREA(lsm_test_thread_wa, 256);
static THD_FUNCTION(lsm_test_thread_fct, p){
  (void)p;
  SEGGER_RTT_printf(0, "\n** TEST MAX5105 **\n");
  SEGGER_RTT_printf(0, "  WRITE DAC0: 0x%02X%02X\n", MAX_WRITE | MAX_DAC0_ADDR , 0xAB);
  SEGGER_RTT_printf(0, "  WR and UPDATE DAC1: 0x%02X%02X\n", MAX_WR_UPD | MAX_DAC1_ADDR, 0xCD);
  SEGGER_RTT_printf(0, "  READ DAC2 - 0x%02X\n" , MAX_READ | MAX_DAC2_ADDR);
  SEGGER_RTT_printf(0, "  LOAD DAC3 - 0x%02X\n" , MAX_LOAD | MAX_DAC3_ADDR);
  SEGGER_RTT_printf(0, "  WRITE SD_MUTE - 0x%02X%02X\n", MAX_WRITE | MAX_SD_MUTE_ADDR , 0xF0);

  while(1){
    txbuf[0] = MAX_WRITE | MAX_DAC0_ADDR;
    txbuf[1] = 0xAB;
    lsm_max5105_swap_bufs();
    txbuf[0] = MAX_WR_UPD | MAX_DAC1_ADDR;
    txbuf[1] = 0xCD;
    lsm_max5105_swap_bufs();
    txbuf[0] = MAX_READ | MAX_DAC2_ADDR;
    txbuf[1] = 0xEE;
    lsm_max5105_swap_bufs();
    txbuf[0] = MAX_LOAD | MAX_DAC3_ADDR;
    txbuf[1] = 0xEE;
    lsm_max5105_swap_bufs();
    txbuf[0] = MAX_WRITE | MAX_SD_MUTE_ADDR;
    txbuf[1] = 0xF0;
    lsm_max5105_swap_bufs();
  }
}

void lsm_max5105_test(void){
  chThdCreateStatic(lsm_test_thread_wa, sizeof(lsm_test_thread_wa),
                    NORMALPRIO + 1, lsm_test_thread_fct, NULL);
}

#ifndef NO_HARDWARE_MUTE
void lsm_max5105_hw_muteX(uint8_t mute) {
  if (mute) {
    palSetLine(MUTE_LINE);
  } else {
    palClearLine(MUTE_LINE);
  }
}
#endif // !NO_HARDWARE_MUTE
