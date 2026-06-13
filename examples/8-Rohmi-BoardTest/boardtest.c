#include "board.h"
#include "led.h"

#include <libstefi/i2c.h>
#include <libstefi/systick.h>
#include <stdio.h>

/*
 * Rohmi (NUCLEO-G431KB) board smoke test: LSM6DSO IMU on I2C2
 * (SCL = PA9, SDA = PA8, addr 0x6B per board.c — 0x6A if SA0 is low).
 *
 * Prints WHO_AM_I (must be 0x6C), then streams accel [mg] and gyro [mdps]
 * at 2 Hz on the VCP (115200 baud). Board flat on the desk: expect
 * z = +1000 mg, x = y = 0, gyro = 0 (small offsets are normal).
 *
 * The I2C driver blocks without timeout on NACK: if the output stops at
 * "probing", the address (SA0 strap) or the wiring is wrong.
 */

#define IMU_I2C_ID   2     //1-based HAL id: I2C2 (NOT the 0-based enum!)
#define IMU_ADDR     0x6B  //retry SA0-high addr now that the bus is alive

#define REG_WHO_AM_I 0x0F
#define REG_CTRL1_XL 0x10
#define REG_CTRL2_G  0x11
#define REG_OUTX_L_G 0x22  //gyro x/y/z then accel x/y/z, auto-increment

static uint8_t reg_read(uint8_t reg) {
    uint8_t v = 0;
    i2c_writeto(IMU_I2C_ID, IMU_ADDR, &reg, 1, true);
    i2c_readfrom(IMU_I2C_ID, IMU_ADDR, &v, 1);
    return v;
}

static void reg_read_burst(uint8_t reg, uint8_t *buf, uint32_t len) {
    i2c_writeto(IMU_I2C_ID, IMU_ADDR, &reg, 1, true);
    i2c_readfrom(IMU_I2C_ID, IMU_ADDR, buf, len);
}

static void reg_write(uint8_t reg, uint8_t val) {
    i2c_writeto_reg(IMU_I2C_ID, IMU_ADDR, reg, &val, 1);
}

void main(void) {
    board_init();
    //experiment: bus reads low — try the internal ~40k pull-ups in case the
    //board has no external ones (works at 100 kHz on short traces)
    gpio_set_pupd(imu.scl, PULL_UP);
    gpio_set_pupd(imu.sda, PULL_UP);
    i2c_init(IMU_I2C_ID);

    //DIAGNOSTIC HACK (remove once the pull-up solder jumper is closed):
    //the ~40k internal pull-ups give ~2us rise times — too slow for 100 kHz.
    //Drop I2C2 to 10 kHz (RM0440 example value for 16 MHz I2CCLK) so the
    //sluggish edges still satisfy the bus timing. Direct register poke,
    //since i2c_init hardcodes the 100 kHz TIMINGR.
    {
        volatile uint32_t *i2c2_cr1     = (uint32_t *)0x40005800;
        volatile uint32_t *i2c2_timingr = (uint32_t *)0x40005810;
        *i2c2_cr1 &= ~1U;               //PE=0: TIMINGR writable
        *i2c2_timingr = 0x3042C3C7;     //10 kHz @ 16 MHz
        *i2c2_cr1 |= 1U;                //PE=1
    }

    printf("\r\nRohmi board test: LSM6DSO IMU on I2C2 \r\n");
    printf("probing addr 0x%02X \r\n", IMU_ADDR);

    uint8_t who = reg_read(REG_WHO_AM_I);
    printf("WHO_AM_I = 0x%02X (expect 0x6C) \r\n", who);

    reg_write(REG_CTRL1_XL, 0x40); //accel 104 Hz, +/-2g
    reg_write(REG_CTRL2_G,  0x40); //gyro  104 Hz, 250 dps

    while (1) {
        led_toggle(LED_NUCLEO);

        uint8_t raw[12];
        reg_read_burst(REG_OUTX_L_G, raw, 12);
        int16_t gx = (int16_t)((raw[1] << 8) | raw[0]);
        int16_t gy = (int16_t)((raw[3] << 8) | raw[2]);
        int16_t gz = (int16_t)((raw[5] << 8) | raw[4]);
        int16_t ax = (int16_t)((raw[7] << 8) | raw[6]);
        int16_t ay = (int16_t)((raw[9] << 8) | raw[8]);
        int16_t az = (int16_t)((raw[11] << 8) | raw[10]);

        //+/-2g: 0.061 mg/LSB; 250 dps: 8.75 mdps/LSB
        printf("acc[mg] x=%5d y=%5d z=%5d | gyro[mdps] x=%7d y=%7d z=%7d \r\n",
               ax * 61 / 1000, ay * 61 / 1000, az * 61 / 1000,
               gx * 875 / 100, gy * 875 / 100, gz * 875 / 100);

        systick_delay_ms(500);
    }
}
