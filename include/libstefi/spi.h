#pragma once

#include <stdint.h>

#include "libstefi/peripheral.h"
#include "libstefi/gpio.h"

/**********************************************************************/
/* SPI Hardware Abstraction layer */
/**********************************************************************/

/* SPI initialization functions*/
void spi_init(const peripheral_spi_t spi_id);
void spi_gpio_init(gpio_id_t cs_pin, gpio_id_t sck_pin, gpio_id_t miso_pin, gpio_id_t mosi_pin);

/* SPI processing functions: SPI full duplex read, write*/
uint8_t spi_transfer(const peripheral_spi_t spi_id, uint8_t data);
