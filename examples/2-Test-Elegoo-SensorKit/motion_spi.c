#include <libstefi/spi.h>
#include <libstefi/uart.h>
#include <libstefi/util.h>

#include "libstefi/gpio.h"
#include "libstefi/peripheral.h"


/* Sensor: MPU 6500 using SPI  */
/* Demo: Connect VCC-3.3 V, GND */
/* Function: get acceleration gyro information from MPU-6050 (I2C, SPI)    */

#define NCS A4 //
#define SCK A5
#define MISO A6
#define MOSI A7 //

int16_t accelerometer_x, accelerometer_y, accelerometer_z;
int16_t gyro_x, gyro_y, gyro_z;
int16_t temperature;

void mpu6500_write_reg(uint8_t reg, uint8_t data) {
    gpio_write(A4, LOW);
    spi_transfer(PERIPHERAL_SPI1, reg & 0x7F);
    spi_transfer(PERIPHERAL_SPI1, data);
    gpio_write(A4, HIGH);
}

uint8_t mpu6500_read_reg(uint8_t reg) {
    uint8_t data;
    gpio_write(A4, LOW);
    spi_transfer(PERIPHERAL_SPI1, reg | 0x80);
    data = spi_transfer(PERIPHERAL_SPI1, 0xFF);
    gpio_write(A4, HIGH);
    return data;
}

int16_t mpu6500_read_val(uint8_t reg) {
    uint8_t hi, lo;
    gpio_write(A4, LOW);
    spi_transfer(PERIPHERAL_SPI1, reg | 0x80);
    hi = spi_transfer(PERIPHERAL_SPI1, 0xFF);
    lo = spi_transfer(PERIPHERAL_SPI1, 0xFF);
    gpio_write(A4, HIGH);
    return ((hi << 8) | lo);
}

void mpu6500_init() {
    mpu6500_write_reg(0x6B, 0X00); //Wakeup
    soft_delay_ms(100);
    mpu6500_write_reg(0x1B, 0X00);//Gyro conf
    mpu6500_write_reg(0x1C, 0X00);//Accel conf
}

void setup_mpu6500_spi() {
    printf("\r\n SPI MPU6500 Init, \r\n");
    spi_init(PERIPHERAL_SPI1);
    //SDI:MOSI SD0:MISO
    spi_gpio_init(A4,A5, A6, A7);
    mpu6500_init();
}

void main(){
    uart_configure();

    setup_mpu6500_spi();

    while(1) {
        accelerometer_x  = mpu6500_read_val(0x3B);
        accelerometer_y  = mpu6500_read_val(0x3D);
        accelerometer_z  = mpu6500_read_val(0x3F);
        temperature = mpu6500_read_val(0x41);
        gyro_x   = mpu6500_read_val(0x43);
        gyro_y   = mpu6500_read_val(0x45);
        gyro_z   = mpu6500_read_val(0x47);
        printf("Acc [X/Y/Z]: %d / %d / %d \r\n", accelerometer_x, accelerometer_y, accelerometer_z);
        printf("Gyro [X/Y/Z]: %d / %d / %d \r\n", gyro_x, gyro_y, gyro_z);
        printf("Temp (C): %.2f \r\n", 0.002995*temperature + 21);
        soft_delay_ms(1000);
    }
}