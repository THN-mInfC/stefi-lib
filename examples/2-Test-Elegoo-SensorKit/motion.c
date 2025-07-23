#include <libstefi/i2c.h>
#include <libstefi/uart.h>
#include <libstefi/util.h>

#include "libstefi/gpio.h"
#include "libstefi/peripheral.h"
#include <libstefi/systick.h>
#include "libstefi/timer.h"

/* Sensor: MPU-6500  */
/* Demo: Connect VCC-3.3 V, GND */
/* Function: get acceleration gyro information from MPU-6050 (I2C, SPI)    */

#define USER_LED A5
#define US_IN_PIN C9 //echo
#define US_OUT_PIN A1

const int MPU_ADDR = 0x68; // I2C address of the MPU-6050.

int16_t accelerometer_x, accelerometer_y, accelerometer_z;
int16_t gyro_x, gyro_y, gyro_z;
int16_t temperature;

#define I2C_ID 2
#define SCL B10
#define SDA B11

void main(){
    uart_configure();

    i2c_gpio_init(SCL, SDA);
    i2c_init(I2C_ID);

    uint8_t whoami;
    i2c_writeto(I2C_ID, MPU_ADDR, (uint8_t[]){0x75}, 1, true);
    i2c_readfrom(I2C_ID, MPU_ADDR, &whoami, 1);
    printf("%x \r\n", whoami);

    uint8_t byte = 0x00;

    //Wake up MPU
    i2c_writeto_reg(I2C_ID, MPU_ADDR, 0x6B, &byte, 1);


    while(1) {
        uint8_t buf[14];

        // Write register address, then read 14 bytes
        uint8_t reg = 0x3B;
        i2c_writeto(I2C_ID, MPU_ADDR, &reg, 1, true);
        i2c_readfrom(I2C_ID, MPU_ADDR, buf, 14);

        // Convert big-endian to int16
        accelerometer_x  = (int16_t)(buf[0] << 8 | buf[1]);
        accelerometer_y  = (int16_t)(buf[2] << 8 | buf[3]);
        accelerometer_z  = (int16_t)(buf[4] << 8 | buf[5]);
        temperature = (int16_t)(buf[6] << 8 | buf[7]);
        gyro_x   = (int16_t)(buf[8] << 8 | buf[9]);
        gyro_y   = (int16_t)(buf[10] << 8 | buf[11]);
        gyro_z   = (int16_t)(buf[12] << 8 | buf[13]);
        printf("Acc [X/Y/Z]: %d / %d / %d \r\n", accelerometer_x, accelerometer_y, accelerometer_z);
        printf("Gyro [X/Y/Z]: %d / %d / %d \r\n", gyro_x, gyro_y, gyro_z);
        printf("Temp (C): %.2f \r\n", 0.002995*temperature + 21);
        soft_delay_ms(1000);
    }
}