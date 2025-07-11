#pragma once
#include <stdint.h>

typedef struct
{
    volatile uint32_t CR1;         /*!< SPI Control register 1,                              Address offset: 0x00 */
    volatile uint32_t CR2;         /*!< SPI Control register 2,                              Address offset: 0x04 */
    volatile uint32_t SR;          /*!< SPI Status register,                                 Address offset: 0x08 */
    volatile uint32_t DR;          /*!< SPI data register,                                  Address offset: 0x0C */
    volatile uint32_t CRCPR;       /*!< SPI CRC polynomial register,                         Address offset: 0x10 */
    volatile uint32_t RXCRCR;      /*!< SPI Rx CRC register,                                 Address offset: 0x14 */
    volatile uint32_t TXCRCR;      /*!< SPI Tx CRC register,                                 Address offset: 0x18 */
    volatile uint32_t I2SCFGR;     /*!< SPI_I2S configuration register,                      Address offset: 0x1C */
    volatile uint32_t I2SPR;       /*!< SPI_I2S prescaler register,                          Address offset: 0x20 */
} SPI_t;


#define SPI1_BASE ((SPI_t *) 0x40013000)
