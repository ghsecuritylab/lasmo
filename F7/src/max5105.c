#include "ch.h"
#include "hal.h"

#include "RTT/SEGGER_RTT.h"
#include "RTT/SEGGER_RTT_Conf.h"

#include "F7/max5105.h"

static const SPIConfig hs_spicfg = {
  false,
  NULL,
  SPI1_NSS_GPIO,
  SPI1_NSS_PIN,
  SPI_CR1_BR_2 | SPI_CR1_BR_1,
  /* SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0 */
  0x700
};

#define SPI_BUFFERS_SIZE    2
static uint8_t txbuf[SPI_BUFFERS_SIZE];
static uint8_t rxbuf[SPI_BUFFERS_SIZE];

void lsm_max5105_init(void){
  palSetPadMode(SPI1_NSS_GPIO , SPI1_NSS_PIN , PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(SPI1_MOSI_GPIO, SPI1_MOSI_PIN, PAL_MODE_ALTERNATE(5)   );
  palSetPadMode(SPI1_MISO_GPIO, SPI1_MISO_PIN, PAL_MODE_ALTERNATE(5)   );
  palSetPadMode(SPI1_SCK_GPIO , SPI1_SCK_PIN , PAL_MODE_ALTERNATE(5)   );
}

void lsm_max5105_swap_bufs(void){
  spiAcquireBus(&SPID1);
  spiStart(&SPID1, &hs_spicfg);

  spiSelect(&SPID1);
  spiExchange(&SPID1, SPI_BUFFERS_SIZE, txbuf, rxbuf);
  spiUnselect(&SPID1);

  spiReleaseBus(&SPID1);
}

void lsm_max5105_test(void){
  while(1){
    txbuf[0] = 0xAB;
    txbuf[1] = 0xCD;
    lsm_max5105_swap_bufs();
    txbuf[0] = 0xFF;
    txbuf[1] = 0xFF;
    lsm_max5105_swap_bufs();
  }
}

