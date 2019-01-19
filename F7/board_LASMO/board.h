/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef BOARD_H
#define BOARD_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*
 * Setup for STMicroelectronics STM32F767VIT - LASMO board.
 */

/*
 * Board identifier.
 */
#define BOARD_ST_STM32F767VIT_LASMO
#define BOARD_NAME                  "STMicroelectronics STM32F767VIT - LASMO"

/*
 * Ethernet PHY type.
 */
#define BOARD_PHY_ID                MII_LAN8742A_ID
#define BOARD_PHY_RMII

/*
 * The board has an ULPI USB PHY.
 */
// #define BOARD_OTG2_USES_ULPI

/*
 * Board oscillators-related settings.
 */
//TODO
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768U
#endif

#define STM32_LSEDRV                (3U << 3U)

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                8000000U
#endif

#define STM32_HSE_BYPASS

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   330U

/*
 * MCU type as defined in the ST header.
 */
#define STM32F767xx

/*
 * IO pins assignments.
 */
#define GPIOA_PHY_RSTn              0U
#define GPIOA_RMII_REF_CLK          1U
#define GPIOA_RMII_MDIO             2U
#define GPIOA_PIN3                  3U
#define GPIOA_DAC_OUT1              4U
#define GPIOA_DAC_OUT2              5U
#define GPIOA_PIN6                  6U
#define GPIOA_RMII_CRS_DV           7U
#define GPIOA_PIN8                  8U
#define GPIOA_PIN9                  9U
#define GPIOA_PIN10                 10U
#define GPIOA_SPI2_NSS              11U
#define GPIOA_SPI2_SCK              12U
#define GPIOA_SWDIO                 13U
#define GPIOA_SWCLK                 14U
#define GPIOA_SPI1_NSS              15U

#define GPIOB_ADC12_IN8             0U
#define GPIOB_ADC12_IN9             1U
#define GPIOB_PIN2                  2U
#define GPIOB_SPI1_SCK              3U
#define GPIOB_SPI1_MISO             4U
#define GPIOB_SPI1_MOSI             5U
#define GPIOB_PIN6                  6U
#define GPIOB_PIN7                  7U
#define GPIOB_PIN8                  8U
#define GPIOB_PIN9                  9U
#define GPIOB_PIN10                 10U
#define GPIOB_RMII_TX_EN            11U
#define GPIOB_RMII_TXD0             12U
#define GPIOB_RMII_TXD1             13U
#define GPIOB_SPI2_MISO             14U
#define GPIOB_SPI2_MOSI             15U

#define GPIOC_PIN0                  0U
#define GPIOC_RMII_MDC              1U
#define GPIOC_PIN2                  2U
#define GPIOC_PIN3                  3U
#define GPIOC_RMII_RXD0             4U
#define GPIOC_RMII_RXD1             5U
#define GPIOC_PIN6                  6U
#define GPIOC_PIN7                  7U
#define GPIOC_SD_D0                 8U
#define GPIOC_SD_D1                 9U
#define GPIOC_SD_D2                 10U
#define GPIOC_SD_D3                 11U
#define GPIOC_SD_CK                 12U
#define GPIOC_PIN13                 13U
#define GPIOC_OSC32_IN              14U
#define GPIOC_OSC32_OUT             15U

#define GPIOD_UART4_RX              0U
#define GPIOD_UART4_TX              1U
#define GPIOD_SD_CMD                2U
#define GPIOD_PIN3                  3U
#define GPIOD_PIN4                  4U
#define GPIOD_PIN5                  5U
#define GPIOD_PIN6                  6U
#define GPIOD_PIN7                  7U
#define GPIOD_PIN8                  8U
#define GPIOD_PIN9                  9U
#define GPIOD_PIN10                 10U
#define GPIOD_PIN11                 11U
#define GPIOD_PIN12                 12U
#define GPIOD_PIN13                 13U
#define GPIOD_PIN14                 14U
#define GPIOD_PIN15                 15U

#define GPIOE_PIN0                  0U
#define GPIOE_PIN1                  1U
#define GPIOE_PIN2                  2U
#define GPIOE_PIN3                  3U
#define GPIOE_PIN4                  4U
#define GPIOE_PIN5                  5U
#define GPIOE_PIN6                  6U
#define GPIOE_PIN7                  7U
#define GPIOE_PIN8                  8U
#define GPIOE_PIN9                  9U
#define GPIOE_PIN10                 10U
#define GPIOE_PIN11                 11U
#define GPIOE_PIN12                 12U
#define GPIOE_PIN13                 13U
#define GPIOE_PIN14                 14U
#define GPIOE_PIN15                 15U

/*
 * IO lines assignments.
 */
#define LINE_PHY_RSTn               PAL_LINE(GPIOA, 0U)
#define LINE_RMII_REF_CLK           PAL_LINE(GPIOA, 1U)
#define LINE_RMII_MDIO              PAL_LINE(GPIOA, 2U)
#define LINE_DAC_OUT1               PAL_LINE(GPIOA, 4U)
#define LINE_DAC_OUT2               PAL_LINE(GPIOA, 5U)
#define LINE_RMII_CRS_DV            PAL_LINE(GPIOA, 7U)
#define LINE_SPI2_NSS               PAL_LINE(GPIOA, 11U)
#define LINE_SPI2_SCK               PAL_LINE(GPIOA, 12U)
#define LINE_SWDIO                  PAL_LINE(GPIOA, 13U)
#define LINE_SWCLK                  PAL_LINE(GPIOA, 14U)
#define LINE_SPI1_NSS               PAL_LINE(GPIOA, 15U)
#define LINE_ADC12_IN8              PAL_LINE(GPIOB, 0U)
#define LINE_ADC12_IN9              PAL_LINE(GPIOB, 1U)
#define LINE_SPI1_SCK               PAL_LINE(GPIOB, 3U)
#define LINE_SPI1_MISO              PAL_LINE(GPIOB, 4U)
#define LINE_SPI1_MOSI              PAL_LINE(GPIOB, 5U)
#define LINE_RMII_TX_EN             PAL_LINE(GPIOB, 11U)
#define LINE_RMII_TXD0              PAL_LINE(GPIOB, 12U)
#define LINE_RMII_TXD1              PAL_LINE(GPIOB, 13U)
#define LINE_SPI2_MISO              PAL_LINE(GPIOB, 14U)
#define LINE_SPI2_MOSI              PAL_LINE(GPIOB, 15U)
#define LINE_RMII_MDC               PAL_LINE(GPIOC, 1U)
#define LINE_RMII_RXD0              PAL_LINE(GPIOC, 4U)
#define LINE_RMII_RXD1              PAL_LINE(GPIOC, 5U)
#define LINE_SD_D0                  PAL_LINE(GPIOC, 8U)
#define LINE_SD_D1                  PAL_LINE(GPIOC, 9U)
#define LINE_SD_D2                  PAL_LINE(GPIOC, 10U)
#define LINE_SD_D3                  PAL_LINE(GPIOC, 11U)
#define LINE_SD_CK                  PAL_LINE(GPIOC, 12U)
#define LINE_OSC32_IN               PAL_LINE(GPIOC, 14U)
#define LINE_OSC32_OUT              PAL_LINE(GPIOC, 15U)
#define LINE_UART4_RX               PAL_LINE(GPIOD, 0U)
#define LINE_UART4_TX               PAL_LINE(GPIOD, 1U)
#define LINE_SD_CMD                 PAL_LINE(GPIOD, 2U)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

/*
 * GPIOA setup:
 * PA0  - PHY_RSTn              (input floating).
 * PA1  - RMII_REF_CLK          (alternate 11).
 * PA2  - RMII_MDIO             (alternate 11).
 * PA3  - PIN3                  (input pullup).
 * PA4  - DAC_OUT1              (output pushpull maximum).
 * PA5  - DAC_OUT2              (output pushpull maximum).
 * PA6  - PIN6                  (input pullup).
 * PA7  - RMII_CRS_DV           (alternate 11).
 * PA8  - PIN8                  (input pullup).
 * PA9  - PIN9                  (input pullup).
 * PA10 - PIN10                 (input pullup).
 * PA11 - SPI2_NSS              (output pushpull maximum).
 * PA12 - SPI2_SCK              (alternate 5).
 * PA13 - SWDIO                 (alternate 0).
 * PA14 - SWCLK                 (alternate 0).
 * PA15 - SPI1_NSS              (output pushpull maximum).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_PHY_RSTn)         | \
                                     PIN_MODE_ALTERNATE(GPIOA_RMII_REF_CLK) | \
                                     PIN_MODE_ALTERNATE(GPIOA_RMII_MDIO)    | \
                                     PIN_MODE_INPUT(GPIOA_PIN3)             | \
                                     PIN_MODE_OUTPUT(GPIOA_DAC_OUT1)        | \
                                     PIN_MODE_OUTPUT(GPIOA_DAC_OUT2)        | \
                                     PIN_MODE_INPUT(GPIOA_PIN6)             | \
                                     PIN_MODE_ALTERNATE(GPIOA_RMII_CRS_DV)  |\
                                     PIN_MODE_INPUT(GPIOA_PIN8)             | \
                                     PIN_MODE_INPUT(GPIOA_PIN9)             | \
                                     PIN_MODE_INPUT(GPIOA_PIN10)            | \
                                     PIN_MODE_OUTPUT(GPIOA_SPI2_NSS)        | \
                                     PIN_MODE_ALTERNATE(GPIOA_SPI2_SCK)     | \
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO)        | \
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK)        | \
                                     PIN_MODE_OUTPUT(GPIOA_SPI1_NSS)        )
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_PHY_RSTn)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_RMII_REF_CLK) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_RMII_MDIO)    | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN3)         | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_DAC_OUT1)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_DAC_OUT2)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN6)         | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_RMII_CRS_DV)  | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN8)         | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN9)         | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN10)        | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPI2_NSS)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPI2_SCK)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDIO)        | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWCLK)        | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SPI1_NSS)       )
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_HIGH(GPIOA_PHY_RSTn)        | \
                                     PIN_OSPEED_HIGH(GPIOA_RMII_REF_CLK)    | \
                                     PIN_OSPEED_HIGH(GPIOA_RMII_MDIO)       | \
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN3)         | \
                                     PIN_OSPEED_HIGH(GPIOA_DAC_OUT1)        | \
                                     PIN_OSPEED_HIGH(GPIOA_DAC_OUT2)        | \
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN6)         | \
                                     PIN_OSPEED_VERYLOW(GPIOA_RMII_CRS_DV)  | \
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN8)         | \
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN9)         | \
                                     PIN_OSPEED_VERYLOW(GPIOA_PIN10)        | \
                                     PIN_OSPEED_HIGH(GPIOA_SPI2_NSS)        | \
                                     PIN_OSPEED_HIGH(GPIOA_SPI2_SCK)        | \
                                     PIN_OSPEED_HIGH(GPIOA_SWDIO)           | \
                                     PIN_OSPEED_HIGH(GPIOA_SWCLK)           | \
                                     PIN_OSPEED_HIGH(GPIOA_SPI1_NSS)        )
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_PHY_RSTn)     | \
                                     PIN_PUPDR_FLOATING(GPIOA_RMII_REF_CLK) | \
                                     PIN_PUPDR_PULLUP(GPIOA_RMII_MDIO)      | \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN3)           | \
                                     PIN_PUPDR_PULLUP(GPIOA_DAC_OUT1)       | \
                                     PIN_PUPDR_PULLUP(GPIOA_DAC_OUT2)       | \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN6)           | \
                                     PIN_PUPDR_FLOATING(GPIOA_RMII_CRS_DV)  | \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN8)           | \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN9)           | \
                                     PIN_PUPDR_PULLUP(GPIOA_PIN10)          | \
                                     PIN_PUPDR_FLOATING(GPIOA_SPI2_NSS)     | \
                                     PIN_PUPDR_FLOATING(GPIOA_SPI2_SCK)     | \
                                     PIN_PUPDR_FLOATING(GPIOA_SWDIO)        | \
                                     PIN_PUPDR_FLOATING(GPIOA_SWCLK)        | \
                                     PIN_PUPDR_FLOATING(GPIOA_SPI1_NSS)     )
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_PHY_RSTn)           | \
                                     PIN_ODR_HIGH(GPIOA_RMII_REF_CLK)       | \
                                     PIN_ODR_HIGH(GPIOA_RMII_MDIO)          | \
                                     PIN_ODR_HIGH(GPIOA_PIN3)               | \
                                     PIN_ODR_HIGH(GPIOA_DAC_OUT1)           | \
                                     PIN_ODR_HIGH(GPIOA_DAC_OUT2)           | \
                                     PIN_ODR_HIGH(GPIOA_PIN6)               | \
                                     PIN_ODR_HIGH(GPIOA_RMII_CRS_DV)        | \
                                     PIN_ODR_HIGH(GPIOA_PIN8)               | \
                                     PIN_ODR_HIGH(GPIOA_PIN9)               | \
                                     PIN_ODR_HIGH(GPIOA_PIN10)              | \
                                     PIN_ODR_HIGH(GPIOA_SPI2_NSS)           | \
                                     PIN_ODR_HIGH(GPIOA_SPI2_SCK)           | \
                                     PIN_ODR_HIGH(GPIOA_SWDIO)              | \
                                     PIN_ODR_HIGH(GPIOA_SWCLK)              | \
                                     PIN_ODR_HIGH(GPIOA_SPI1_NSS)             )
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_PHY_RSTn, 0U)        | \
                                     PIN_AFIO_AF(GPIOA_RMII_REF_CLK, 11U)   | \
                                     PIN_AFIO_AF(GPIOA_RMII_MDIO, 11U)      | \
                                     PIN_AFIO_AF(GPIOA_PIN3, 0U)            | \
                                     PIN_AFIO_AF(GPIOA_DAC_OUT1, 0U)        | \
                                     PIN_AFIO_AF(GPIOA_DAC_OUT2, 0U)        | \
                                     PIN_AFIO_AF(GPIOA_PIN6, 0U)            | \
                                     PIN_AFIO_AF(GPIOA_RMII_CRS_DV, 11U)      )
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_PIN8, 0U)            | \
                                     PIN_AFIO_AF(GPIOA_PIN9, 0U)            | \
                                     PIN_AFIO_AF(GPIOA_PIN10, 0U)           | \
                                     PIN_AFIO_AF(GPIOA_SPI2_NSS, 0U)        | \
                                     PIN_AFIO_AF(GPIOA_SPI2_SCK, 5U)        | \
                                     PIN_AFIO_AF(GPIOA_SWDIO, 0U)           | \
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0U)           | \
                                     PIN_AFIO_AF(GPIOA_SPI1_NSS, 0U)          )

/*
 * GPIOB setup:
 *
 * PB0  - ADC12_IN8          (analog).
 * PB1  - ADC12_IN9          (analog).
 * PB2  - PIN2               (input pullup).
 * PB3  - SPI1_SCK           (alternate 5).
 * PB4  - SPI1_MISO          (alternate 5).
 * PB5  - SPI1_MOSI          (alternate 5).
 * PB6  - PIN6               (input pullup).
 * PB7  - PIN7               (input pullup).
 * PB8  - PIN8               (input pullup).
 * PB9  - PIN9               (input pullup).
 * PB10 - PIN10              (input pullup).
 * PB11 - RMII_TX_EN         (alternate 11).
 * PB12 - RMII_TXD0          (alternate 11).
 * PB13 - RMII_TXD1          (alternate 11).
 * PB14 - SPI2_MISO          (alternate 5).
 * PB15 - SPI2_MOSI          (alternate 5).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_ANALOG(GPIOB_ADC12_IN8)     | \
                                     PIN_MODE_ANALOG(GPIOB_ADC12_IN9)     | \
                                     PIN_MODE_INPUT(GPIOB_PIN2)           | \
                                     PIN_MODE_ALTERNATE(GPIOB_SPI1_SCK)   | \
                                     PIN_MODE_ALTERNATE(GPIOB_SPI1_MISO)  | \
                                     PIN_MODE_ALTERNATE(GPIOB_SPI1_MOSI)  | \
                                     PIN_MODE_INPUT(GPIOB_PIN6)           | \
                                     PIN_MODE_INPUT(GPIOB_PIN7)           | \
                                     PIN_MODE_INPUT(GPIOB_PIN8)           | \
                                     PIN_MODE_INPUT(GPIOB_PIN9)           | \
                                     PIN_MODE_INPUT(GPIOB_PIN10)          | \
                                     PIN_MODE_ALTERNATE(GPIOB_RMII_TX_EN) | \
                                     PIN_MODE_ALTERNATE(GPIOB_RMII_TXD0)  | \
                                     PIN_MODE_ALTERNATE(GPIOB_RMII_TXD1)  | \
                                     PIN_MODE_ALTERNATE(GPIOB_SPI2_MISO)  | \
                                     PIN_MODE_ALTERNATE(GPIOB_SPI2_MOSI)    )
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_ADC12_IN8)  | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_ADC12_IN9)  | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN2)       | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SPI1_SCK)   | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SPI1_MISO)  | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SPI1_MOSI)  | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN6)       | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN7)       | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN8)       | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN9)       | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN10)      | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_RMII_TX_EN) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_RMII_TXD0)  | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_RMII_TXD1)  | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SPI2_MISO)  | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SPI2_MOSI)    )
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_HIGH(GPIOB_ADC12_IN8)  | \
                                     PIN_OSPEED_HIGH(GPIOB_ADC12_IN9)  | \
                                     PIN_OSPEED_HIGH(GPIOB_PIN2)       | \
                                     PIN_OSPEED_HIGH(GPIOB_SPI1_SCK)   | \
                                     PIN_OSPEED_HIGH(GPIOB_SPI1_MISO)  | \
                                     PIN_OSPEED_HIGH(GPIOB_SPI1_MOSI)  | \
                                     PIN_OSPEED_HIGH(GPIOB_PIN6)       | \
                                     PIN_OSPEED_HIGH(GPIOB_PIN7)       | \
                                     PIN_OSPEED_HIGH(GPIOB_PIN8)       | \
                                     PIN_OSPEED_HIGH(GPIOB_PIN9)       | \
                                     PIN_OSPEED_HIGH(GPIOB_PIN10)      | \
                                     PIN_OSPEED_HIGH(GPIOB_RMII_TX_EN) | \
                                     PIN_OSPEED_HIGH(GPIOB_RMII_TXD0)  | \
                                     PIN_OSPEED_HIGH(GPIOB_RMII_TXD1)  | \
                                     PIN_OSPEED_HIGH(GPIOB_SPI2_MISO)  | \
                                     PIN_OSPEED_HIGH(GPIOB_SPI2_MOSI)    )
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_FLOATING(GPIOB_ADC12_IN8)  | \
                                     PIN_PUPDR_FLOATING(GPIOB_ADC12_IN9)  | \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN2)         | \
                                     PIN_PUPDR_FLOATING(GPIOB_SPI1_SCK)   | \
                                     PIN_PUPDR_FLOATING(GPIOB_SPI1_MISO)  | \
                                     PIN_PUPDR_FLOATING(GPIOB_SPI1_MOSI)  | \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN6)         | \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN7)         | \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN8)         | \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN9)         | \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN10)        | \
                                     PIN_PUPDR_FLOATING(GPIOB_RMII_TX_EN) | \
                                     PIN_PUPDR_FLOATING(GPIOB_RMII_TXD0)  | \
                                     PIN_PUPDR_FLOATING(GPIOB_RMII_TXD1)  | \
                                     PIN_PUPDR_FLOATING(GPIOB_SPI2_MISO)  | \
                                     PIN_PUPDR_FLOATING(GPIOB_SPI2_MOSI)    )
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_ADC12_IN8)        | \
                                     PIN_ODR_HIGH(GPIOB_ADC12_IN9)        | \
                                     PIN_ODR_HIGH(GPIOB_PIN2)             | \
                                     PIN_ODR_HIGH(GPIOB_SPI1_SCK)         | \
                                     PIN_ODR_HIGH(GPIOB_SPI1_MISO)        | \
                                     PIN_ODR_HIGH(GPIOB_SPI1_MOSI)        | \
                                     PIN_ODR_HIGH(GPIOB_PIN6)             | \
                                     PIN_ODR_HIGH(GPIOB_PIN7)             | \
                                     PIN_ODR_HIGH(GPIOB_PIN8)             | \
                                     PIN_ODR_HIGH(GPIOB_PIN9)             | \
                                     PIN_ODR_HIGH(GPIOB_PIN10)            | \
                                     PIN_ODR_HIGH(GPIOB_RMII_TX_EN)       | \
                                     PIN_ODR_HIGH(GPIOB_RMII_TXD0)        | \
                                     PIN_ODR_HIGH(GPIOB_RMII_TXD1)        | \
                                     PIN_ODR_HIGH(GPIOB_SPI2_MISO)        | \
                                     PIN_ODR_HIGH(GPIOB_SPI2_MOSI)          )
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_ADC12_IN8, 0U)     | \
                                     PIN_AFIO_AF(GPIOB_ADC12_IN9, 0U)     | \
                                     PIN_AFIO_AF(GPIOB_PIN2, 0U)          | \
                                     PIN_AFIO_AF(GPIOB_SPI1_SCK, 5U)      | \
                                     PIN_AFIO_AF(GPIOB_SPI1_MISO, 5U)     | \
                                     PIN_AFIO_AF(GPIOB_SPI1_MOSI, 5U)     | \
                                     PIN_AFIO_AF(GPIOB_PIN6, 0U)          | \
                                     PIN_AFIO_AF(GPIOB_PIN7, 0U)            )
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_PIN8, 0U)          | \
                                     PIN_AFIO_AF(GPIOB_PIN9, 0U)          | \
                                     PIN_AFIO_AF(GPIOB_PIN10, 0U)         | \
                                     PIN_AFIO_AF(GPIOB_RMII_TX_EN, 11U)   | \
                                     PIN_AFIO_AF(GPIOB_RMII_TXD0, 11U)    | \
                                     PIN_AFIO_AF(GPIOB_RMII_TXD1, 11U)    | \
                                     PIN_AFIO_AF(GPIOB_SPI2_MISO, 5U)     | \
                                     PIN_AFIO_AF(GPIOB_SPI2_MOSI, 5U)       )

/*
 * GPIOC setup:
 *
 * PC0  - PIN0                      (input pullup)
 * PC1  - RMII_MDC                  (alternate 11).
 * PC2  - PIN2                      (input pullup).
 * PC3  - PIN3                      (input pullup).
 * PC4  - RMII_RXD0                 (alternate 11).
 * PC5  - RMII_RXD1                 (alternate 11).
 * PC6  - PIN6                      (input pullup).
 * PC7  - PIN7                      (input pullup).
 * PC8  - SD_D0                     (alternate 12).
 * PC9  - SD_D1                     (alternate 12).
 * PC10 - SD_D2                     (alternate 12).
 * PC11 - SD_D3                     (alternate 12).
 * PC12 - SD_CK                     (alternate 12).
 * PC13 - PIN13                     (input pullup).
 * PC14 - OSC32_IN                  (input floating).
 * PC15 - OSC32_OUT                 (input floating).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_INPUT(GPIOC_PIN0)             | \
                                     PIN_MODE_ALTERNATE(GPIOC_RMII_MDC)     | \
                                     PIN_MODE_INPUT(GPIOC_PIN2)             | \
                                     PIN_MODE_INPUT(GPIOC_PIN3)             | \
                                     PIN_MODE_ALTERNATE(GPIOC_RMII_RXD0)    | \
                                     PIN_MODE_ALTERNATE(GPIOC_RMII_RXD1)    | \
                                     PIN_MODE_INPUT(GPIOC_PIN6)             | \
                                     PIN_MODE_INPUT(GPIOC_PIN7)             | \
                                     PIN_MODE_ALTERNATE(GPIOC_SD_D0)        | \
                                     PIN_MODE_ALTERNATE(GPIOC_SD_D1)        | \
                                     PIN_MODE_ALTERNATE(GPIOC_SD_D2)        | \
                                     PIN_MODE_ALTERNATE(GPIOC_SD_D3)        | \
                                     PIN_MODE_ALTERNATE(GPIOC_SD_CK)        | \
                                     PIN_MODE_INPUT(GPIOC_PIN13)            | \
                                     PIN_MODE_INPUT(GPIOC_OSC32_IN)         | \
                                     PIN_MODE_INPUT(GPIOC_OSC32_OUT)          )
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_PIN0)         | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_RMII_MDC)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN2)         | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN3)         | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_RMII_RXD0)    | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_RMII_RXD1)    | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN6)         | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN7)         | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D0)        | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D1)        | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D2)        | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D3)        | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_CK)        | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN13)        | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_OSC32_IN)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOC_OSC32_OUT)      )
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_HIGH(GPIOC_PIN0)            | \
                                     PIN_OSPEED_HIGH(GPIOC_RMII_MDC)        | \
                                     PIN_OSPEED_HIGH(GPIOC_PIN2)            | \
                                     PIN_OSPEED_HIGH(GPIOC_PIN3)            | \
                                     PIN_OSPEED_HIGH(GPIOC_RMII_RXD0)       | \
                                     PIN_OSPEED_HIGH(GPIOC_RMII_RXD1)       | \
                                     PIN_OSPEED_HIGH(GPIOC_PIN6)            | \
                                     PIN_OSPEED_HIGH(GPIOC_PIN7)            | \
                                     PIN_OSPEED_HIGH(GPIOC_SD_D0)           | \
                                     PIN_OSPEED_HIGH(GPIOC_SD_D1)           | \
                                     PIN_OSPEED_HIGH(GPIOC_SD_D2)           | \
                                     PIN_OSPEED_HIGH(GPIOC_SD_D3)           | \
                                     PIN_OSPEED_HIGH(GPIOC_SD_CK)           | \
                                     PIN_OSPEED_HIGH(GPIOC_PIN13)           | \
                                     PIN_OSPEED_VERYLOW(GPIOC_OSC32_IN)     | \
                                     PIN_OSPEED_VERYLOW(GPIOC_OSC32_OUT)      )
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_PULLUP(GPIOC_PIN0)           | \
                                     PIN_PUPDR_FLOATING(GPIOC_RMII_MDC)     | \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN2)           | \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN3)           | \
                                     PIN_PUPDR_FLOATING(GPIOC_RMII_RXD0)    | \
                                     PIN_PUPDR_FLOATING(GPIOC_RMII_RXD1)    | \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN6)           | \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN7)           | \
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D0)        | \
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D1)        | \
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D2)        | \
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D3)        | \
                                     PIN_PUPDR_FLOATING(GPIOC_SD_CK)        | \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN13)          | \
                                     PIN_PUPDR_FLOATING(GPIOC_OSC32_IN)     | \
                                     PIN_PUPDR_FLOATING(GPIOC_OSC32_OUT)      )
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(GPIOC_PIN0)               | \
                                     PIN_ODR_HIGH(GPIOC_RMII_MDC)           | \
                                     PIN_ODR_HIGH(GPIOC_PIN2)               | \
                                     PIN_ODR_HIGH(GPIOC_PIN3)               | \
                                     PIN_ODR_HIGH(GPIOC_RMII_RXD0)          | \
                                     PIN_ODR_HIGH(GPIOC_RMII_RXD1)          | \
                                     PIN_ODR_HIGH(GPIOC_PIN6)               | \
                                     PIN_ODR_HIGH(GPIOC_PIN7)               | \
                                     PIN_ODR_HIGH(GPIOC_SD_D0)              | \
                                     PIN_ODR_HIGH(GPIOC_SD_D1)              | \
                                     PIN_ODR_HIGH(GPIOC_SD_D2)              | \
                                     PIN_ODR_HIGH(GPIOC_SD_D3)              | \
                                     PIN_ODR_HIGH(GPIOC_SD_CK)              | \
                                     PIN_ODR_HIGH(GPIOC_PIN13)              | \
                                     PIN_ODR_HIGH(GPIOC_OSC32_IN)           | \
                                     PIN_ODR_HIGH(GPIOC_OSC32_OUT)            )
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_PIN0, 0U)            | \
                                     PIN_AFIO_AF(GPIOC_RMII_MDC, 11U)       | \
                                     PIN_AFIO_AF(GPIOC_PIN2, 0U)            | \
                                     PIN_AFIO_AF(GPIOC_PIN3, 0U)            | \
                                     PIN_AFIO_AF(GPIOC_RMII_RXD0, 11U)      | \
                                     PIN_AFIO_AF(GPIOC_RMII_RXD1, 11U)      | \
                                     PIN_AFIO_AF(GPIOC_PIN6, 0U)            | \
                                     PIN_AFIO_AF(GPIOC_PIN7, 0U)              )
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_SD_D0, 12U)          | \
                                     PIN_AFIO_AF(GPIOC_SD_D1, 12U)          | \
                                     PIN_AFIO_AF(GPIOC_SD_D2, 12U)          | \
                                     PIN_AFIO_AF(GPIOC_SD_D3, 12U)          | \
                                     PIN_AFIO_AF(GPIOC_SD_CK, 12U)          | \
                                     PIN_AFIO_AF(GPIOC_PIN13, 0U)           | \
                                     PIN_AFIO_AF(GPIOC_OSC32_IN, 0U)        | \
                                     PIN_AFIO_AF(GPIOC_OSC32_OUT, 0U)         )

/*
 * GPIOD setup:
 *
 * PD0  - UART4_RX    (alternate 8).
 * PD1  - UART4_TX    (alternate 8)
 * PD2  - SD_CMD      (alternate 12).
 * PD3  - PIN3        (input pullup)
 * PD4  - PIN4        (input pullup)
 * PD5  - PIN5        (input pullup)
 * PD6  - PIN6        (input pullup)
 * PD7  - PIN7        (input pullup)
 * PD8  - PIN8        (input pullup)
 * PD9  - PIN9        (input pullup)
 * PD10 - PIN10       (input pullup)
 * PD11 - PIN11       (input pullup)
 * PD12 - PIN12       (input pullup)
 * PD13 - PIN13       (input pullup)
 * PD14 - PIN14       (input pullup)
 * PD15 - PIN15       (input pullup)
 */
#define VAL_GPIOD_MODER             (PIN_MODE_ALTERNATE(GPIOD_UART4_RX) | \
                                     PIN_MODE_ALTERNATE(GPIOD_UART4_TX) | \
                                     PIN_MODE_ALTERNATE(GPIOD_SD_CMD)   | \
                                     PIN_MODE_INPUT(GPIOD_PIN3)         | \
                                     PIN_MODE_INPUT(GPIOD_PIN4)         | \
                                     PIN_MODE_INPUT(GPIOD_PIN5)         | \
                                     PIN_MODE_INPUT(GPIOD_PIN6)         | \
                                     PIN_MODE_INPUT(GPIOD_PIN7)         | \
                                     PIN_MODE_INPUT(GPIOD_PIN8)         | \
                                     PIN_MODE_INPUT(GPIOD_PIN9)         | \
                                     PIN_MODE_INPUT(GPIOD_PIN10)        | \
                                     PIN_MODE_INPUT(GPIOD_PIN11)        | \
                                     PIN_MODE_INPUT(GPIOD_PIN12)        | \
                                     PIN_MODE_INPUT(GPIOD_PIN13)        | \
                                     PIN_MODE_INPUT(GPIOD_PIN14)        | \
                                     PIN_MODE_INPUT(GPIOD_PIN15)          )
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_UART4_RX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_UART4_TX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_SD_CMD)   | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN3)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN4)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN5)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN6)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN7)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN8)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN9)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN10)    | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN11)    | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN12)    | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN13)    | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN14)    | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN15)      )
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_HIGH(GPIOD_UART4_RX) | \
                                     PIN_OSPEED_HIGH(GPIOD_UART4_TX) | \
                                     PIN_OSPEED_HIGH(GPIOD_SD_CMD)   | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN3)     | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN4)     | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN5)     | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN6)     | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN7)     | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN8)     | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN9)     | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN10)    | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN11)    | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN12)    | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN13)    | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN14)    | \
                                     PIN_OSPEED_HIGH(GPIOD_PIN15)      )
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_FLOATING(GPIOD_UART4_RX) | \
                                     PIN_PUPDR_FLOATING(GPIOD_UART4_TX) | \
                                     PIN_PUPDR_FLOATING(GPIOD_SD_CMD)   | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN3)       | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN4)       | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN5)       | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN6)       | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN7)       | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN8)       | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN9)       | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN10)      | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN11)      | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN12)      | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN13)      | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN14)      | \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN15)        )
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(GPIOD_UART4_RX)       | \
                                     PIN_ODR_HIGH(GPIOD_UART4_TX)       | \
                                     PIN_ODR_HIGH(GPIOD_SD_CMD)         | \
                                     PIN_ODR_HIGH(GPIOD_PIN3)           | \
                                     PIN_ODR_HIGH(GPIOD_PIN4)           | \
                                     PIN_ODR_HIGH(GPIOD_PIN5)           | \
                                     PIN_ODR_HIGH(GPIOD_PIN6)           | \
                                     PIN_ODR_HIGH(GPIOD_PIN7)           | \
                                     PIN_ODR_HIGH(GPIOD_PIN8)           | \
                                     PIN_ODR_HIGH(GPIOD_PIN9)           | \
                                     PIN_ODR_HIGH(GPIOD_PIN10)          | \
                                     PIN_ODR_HIGH(GPIOD_PIN11)          | \
                                     PIN_ODR_HIGH(GPIOD_PIN12)          | \
                                     PIN_ODR_HIGH(GPIOD_PIN13)          | \
                                     PIN_ODR_HIGH(GPIOD_PIN14)          | \
                                     PIN_ODR_HIGH(GPIOD_PIN15)            )
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_UART4_RX, 8U)    | \
                                     PIN_AFIO_AF(GPIOD_UART4_TX, 8U)    | \
                                     PIN_AFIO_AF(GPIOD_SD_CMD, 12U)     | \
                                     PIN_AFIO_AF(GPIOD_PIN3, 0U)        | \
                                     PIN_AFIO_AF(GPIOD_PIN4, 0U)        | \
                                     PIN_AFIO_AF(GPIOD_PIN5, 0U)        | \
                                     PIN_AFIO_AF(GPIOD_PIN6, 0U)        | \
                                     PIN_AFIO_AF(GPIOD_PIN7, 0U)          )
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(GPIOD_PIN8, 0U)        | \
                                     PIN_AFIO_AF(GPIOD_PIN9, 0U)        | \
                                     PIN_AFIO_AF(GPIOD_PIN10, 0U)       | \
                                     PIN_AFIO_AF(GPIOD_PIN11, 0U)       | \
                                     PIN_AFIO_AF(GPIOD_PIN12, 0U)       | \
                                     PIN_AFIO_AF(GPIOD_PIN13, 0U)       | \
                                     PIN_AFIO_AF(GPIOD_PIN14, 0U)       | \
                                     PIN_AFIO_AF(GPIOD_PIN15, 0U)         )

/*
 * GPIOE setup:
 *
 * PE0  - PIN0                  (input pullup).
 * PE1  - PIN1                  (input pullup).
 * PE2  - PIN2                  (input pullup).
 * PE3  - PIN3                  (input pullup).
 * PE4  - PIN4                  (input pullup).
 * PE5  - PIN5                  (input pullup).
 * PE6  - PIN6                  (input pullup).
 * PE7  - PIN7                  (input pullup).
 * PE8  - PIN8                  (input pullup).
 * PE9  - PIN9                  (input pullup).
 * PE10 - PIN10                 (input pullup).
 * PE11 - PIN11                 (input pullup).
 * PE12 - PIN12                 (input pullup).
 * PE13 - PIN13                 (input pullup).
 * PE14 - PIN14                 (input pullup).
 * PE15 - PIN15                 (input pullup).
 */
#define VAL_GPIOE_MODER             (PIN_MODE_INPUT(GPIOE_PIN0)         | \
                                     PIN_MODE_INPUT(GPIOE_PIN1)         | \
                                     PIN_MODE_INPUT(GPIOE_PIN2)         | \
                                     PIN_MODE_INPUT(GPIOE_PIN3)         | \
                                     PIN_MODE_INPUT(GPIOE_PIN4)         | \
                                     PIN_MODE_INPUT(GPIOE_PIN5)         | \
                                     PIN_MODE_INPUT(GPIOE_PIN6)         | \
                                     PIN_MODE_INPUT(GPIOE_PIN7)         | \
                                     PIN_MODE_INPUT(GPIOE_PIN8)         | \
                                     PIN_MODE_INPUT(GPIOE_PIN9)         | \
                                     PIN_MODE_INPUT(GPIOE_PIN10)        | \
                                     PIN_MODE_INPUT(GPIOE_PIN11)        | \
                                     PIN_MODE_INPUT(GPIOE_PIN12)        | \
                                     PIN_MODE_INPUT(GPIOE_PIN13)        | \
                                     PIN_MODE_INPUT(GPIOE_PIN14)        | \
                                     PIN_MODE_INPUT(GPIOE_PIN15)          )
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOE_PIN0)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN1)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN2)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN3)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN4)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN5)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN6)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN7)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN8)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN9)     | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN10)    | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN11)    | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN12)    | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN13)    | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN14)    | \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN15)      )
#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_HIGH(GPIOE_PIN0)        | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN1)        | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN2)        | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN3)        | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN4)        | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN5)        | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN6)        | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN7)        | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN8)        | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN9)        | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN10)       | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN11)       | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN12)       | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN13)       | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN14)       | \
                                     PIN_OSPEED_HIGH(GPIOE_PIN15)         )
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_PULLUP(GPIOE_PIN0)       | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN1)       | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN2)       | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN3)       | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN4)       | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN5)       | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN6)       | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN7)       | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN8)       | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN9)       | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN10)      | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN11)      | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN12)      | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN13)      | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN14)      | \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN15)        )
#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(GPIOE_PIN0)           | \
                                     PIN_ODR_HIGH(GPIOE_PIN1)           | \
                                     PIN_ODR_HIGH(GPIOE_PIN2)           | \
                                     PIN_ODR_HIGH(GPIOE_PIN3)           | \
                                     PIN_ODR_HIGH(GPIOE_PIN4)           | \
                                     PIN_ODR_HIGH(GPIOE_PIN5)           | \
                                     PIN_ODR_HIGH(GPIOE_PIN6)           | \
                                     PIN_ODR_HIGH(GPIOE_PIN7)           | \
                                     PIN_ODR_HIGH(GPIOE_PIN8)           | \
                                     PIN_ODR_HIGH(GPIOE_PIN9)           | \
                                     PIN_ODR_HIGH(GPIOE_PIN10)          | \
                                     PIN_ODR_HIGH(GPIOE_PIN11)          | \
                                     PIN_ODR_HIGH(GPIOE_PIN12)          | \
                                     PIN_ODR_HIGH(GPIOE_PIN13)          | \
                                     PIN_ODR_HIGH(GPIOE_PIN14)          | \
                                     PIN_ODR_HIGH(GPIOE_PIN15)            )
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_PIN0, 0U)        | \
                                     PIN_AFIO_AF(GPIOE_PIN1, 0U)        | \
                                     PIN_AFIO_AF(GPIOE_PIN2, 0U)        | \
                                     PIN_AFIO_AF(GPIOE_PIN3, 0U)        | \
                                     PIN_AFIO_AF(GPIOE_PIN4, 0U)        | \
                                     PIN_AFIO_AF(GPIOE_PIN5, 0U)        | \
                                     PIN_AFIO_AF(GPIOE_PIN6, 0U)        | \
                                     PIN_AFIO_AF(GPIOE_PIN7, 0U)          )
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(GPIOE_PIN8, 0U)        | \
                                     PIN_AFIO_AF(GPIOE_PIN9, 0U)        | \
                                     PIN_AFIO_AF(GPIOE_PIN10, 0U)       | \
                                     PIN_AFIO_AF(GPIOE_PIN11, 0U)       | \
                                     PIN_AFIO_AF(GPIOE_PIN12, 0U)       | \
                                     PIN_AFIO_AF(GPIOE_PIN13, 0U)       | \
                                     PIN_AFIO_AF(GPIOE_PIN14, 0U)       | \
                                     PIN_AFIO_AF(GPIOE_PIN15, 0U)         )

/*===========================================================================*/
/* DAC                                                                       */
/*===========================================================================*/
#define DAC1_GPIO        GPIOA
#define DAC2_GPIO        GPIOA

#define DAC1_PIN         GPIOA_DAC_OUT1
#define DAC2_PIN         GPIOA_DAC_OUT2

/*===========================================================================*/
/* SPI1                                                                      */
/*===========================================================================*/
#define SPI1_NSS_GPIO    GPIOA
#define SPI1_MOSI_GPIO   GPIOB
#define SPI1_MISO_GPIO   GPIOB
#define SPI1_SCK_GPIO    GPIOB

#define SPI1_NSS_PIN     GPIOA_SPI1_NSS
#define SPI1_MOSI_PIN    GPIOB_SPI1_MOSI
#define SPI1_MISO_PIN    GPIOB_SPI1_MISO
#define SPI1_SCK_PIN     GPIOB_SPI1_SCK

/*===========================================================================*/
/* ADC                                                                      */
/*===========================================================================*/

#define ADC_TRIG_ORIG     12
#define ADC_SAMPLE_RATE   ( ADC_SMPR2_SMP_AN8(ADC_SAMPLE_3) | ADC_SMPR2_SMP_AN9(ADC_SAMPLE_3) )
#define ADC_CONV_SEQUENCE ( ADC_SQR3_SQ2_N(ADC_CHANNEL_IN9) | ADC_SQR3_SQ1_N(ADC_CHANNEL_IN8) )

#define ADC_RIGHT_PORT GPIOB
#define ADC_LEFT_PORT  GPIOB
#define ADC_RIGHT_PIN  GPIOB_ADC12_IN8
#define ADC_LEFT_PIN   GPIOB_ADC12_IN9
#define ADC_DRIVER     ADCD1
#define ADC_GPT        GPTD4

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* BOARD_H */
