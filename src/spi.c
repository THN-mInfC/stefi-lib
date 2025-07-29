#include "libstefi/spi.h"

#include <assert.h>
#include <stddef.h>
#include <libstefi/peripheral.h>
#include <libstefi/util.h>

#include "internal/spi_internal.h"

SPI_t *spi_peripherals[] = {(SPI_t*) SPI1_BASE, NULL, NULL};

void spi_init(const peripheral_spi_t spi_id) {
    assert(spi_id >= 0);
    peripheral_spi_enable(spi_id);

    SPI_t *spi = spi_peripherals[spi_id];
    spi->CR1 |= BIT(4); //bitrate = clock/8
    spi->CR1 |= BIT(2); //Master Mode
    spi->CR1 |= BIT(8) | BIT(9);// Software NSS, set SSI

    spi->CR2 |= BIT(12);
    spi->CR1 |= BIT(6);   // SPI enable
}

void spi_gpio_init(gpio_id_t cs_pin, gpio_id_t sck_pin, gpio_id_t miso_pin, gpio_id_t mosi_pin) {
    peripheral_gpio_enable(gpio_get_port_from_portpin(cs_pin));
    peripheral_gpio_enable(gpio_get_port_from_portpin(sck_pin));
    peripheral_gpio_enable(gpio_get_port_from_portpin(miso_pin));
    peripheral_gpio_enable(gpio_get_port_from_portpin(mosi_pin));

    gpio_set_alternate_function(cs_pin, AF5);//all spi on AF5
    gpio_set_alternate_function(sck_pin, AF5);
    gpio_set_alternate_function(miso_pin, AF5);//all spi on AF5
    gpio_set_alternate_function(mosi_pin, AF5);

    gpio_set_mode(cs_pin, MODER_AF);
    gpio_set_mode(sck_pin, MODER_AF);
    gpio_set_mode(miso_pin, MODER_AF);
    gpio_set_mode(mosi_pin, MODER_AF);

    gpio_set_output_speed(sck_pin, HIGH_S);
    gpio_set_output_speed(miso_pin, HIGH_S);
    gpio_set_output_speed(mosi_pin, HIGH_S);

    gpio_set_mode(A4, MODER_OUTPUT);
}

uint8_t spi_transfer(const peripheral_spi_t spi_id, uint8_t data) {
    assert(spi_id >= 0);
    SPI_t *spi = spi_peripherals[spi_id];

    uint8_t rx_data;
    // Wait until TX buffer is empty
    while (!( spi->SR & BIT(1) ));
    *(uint8_t *)&(spi->DR) = data;

    // Wait until RX buffer is not empty
    while (!(spi->SR & BIT(0)));
    rx_data = (uint8_t)spi->DR;
    return rx_data; // Return received data
}