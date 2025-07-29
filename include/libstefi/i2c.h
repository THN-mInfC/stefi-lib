#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "peripheral.h"
#include "libstefi/gpio.h"

/**********************************************************************/
/* I2C Hardware Abstraction layer */
/**********************************************************************/

/* I2C initialization functions*/
void i2c_init(peripheral_i2c_t i2c_id);
void i2c_gpio_init(gpio_id_t scl_pin, gpio_id_t sda_pin);

/* I2C processing functions: readfrom, writeto*/

/**
 * @brief Read (len) byte from an I2C slave device and store in buf.
 * @details This is a blocking call
 * Example: reading 14 bytes from MPU Sensor with I2C Address 0x68
 * @code
 * i2c_readfrom(I2C_ID, MPU_ADDR, buf, 14);
 * @endcode
 */
void i2c_readfrom(peripheral_i2c_t i2c_id, uint8_t address7b, uint8_t *buf, uint32_t len);

/**
 * @brief Write (len) data from buffer to an I2C slave device.
 * @param repeat If, true no stop bit sent => repeated start (needed for atomic write reg/ read data use case)
 * Example:
 * @code
 * i2c_writeto(I2C_ID, LM75_ADDR, &reg_addr, 1, true);
 * i2c_readfrom(I2C_ID, LM75_ADDR, rx_value, 2);
 * @endcode
 */
void i2c_writeto(peripheral_i2c_t i2c_id, uint8_t address7b, uint8_t *buf, uint32_t len, bool repeat);

/**
 * @brief Helper for register-based i2c device. Write (len) byte from buffer to a register (or memory) address of I2C slave device.
 * @details This is a blocking call
 * Example: writing 1 command byte to Register with address 0x6B MPU Sensor with I2C Address 0x68
 * @code
 * i2c_writeto_reg(I2C_ID, MPU_ADDR, 0x6B, &byte, 1);
 * @endcode
 * Example: streaming image bytes to write-only GDDRAM data Register of SSD1306 OLED
 * @code
 * i2c_writeto_reg(I2C_ID, SSD1306_I2C_ADDR, 0x40, buffer, buff_size);
 * @endcode
 */
void i2c_writeto_reg(peripheral_i2c_t i2c_id, uint8_t address7b, uint8_t reg, uint8_t *buf, uint32_t len);