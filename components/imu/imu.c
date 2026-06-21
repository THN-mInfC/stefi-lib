#include "imu.h"

#include "libstefi/i2c.h"

#include <stddef.h>

/*
 * LSM6DSO register map (subset). Multi-byte outputs are little-endian and the
 * device auto-increments the register pointer, so a single burst read from
 * OUTX_L_G returns gyro x/y/z then accel x/y/z (12 bytes).
 */
#define REG_WHO_AM_I 0x0F
#define REG_CTRL1_XL 0x10
#define REG_CTRL2_G  0x11
#define REG_OUTX_L_G 0x22

/* CTRL1_XL / CTRL2_G: 104 Hz ODR, +/-2 g (accel) / 250 dps (gyro). */
#define CFG_CTRL1_XL 0x40
#define CFG_CTRL2_G  0x40

static uint8_t reg_read(uint8_t reg) {
    uint8_t v = 0;
    i2c_writeto(imu.i2c_id, imu.i2c_addr, &reg, 1, true);
    i2c_readfrom(imu.i2c_id, imu.i2c_addr, &v, 1);
    return v;
}

static void reg_write(uint8_t reg, uint8_t val) {
    i2c_writeto_reg(imu.i2c_id, imu.i2c_addr, reg, &val, 1);
}

static void reg_read_burst(uint8_t reg, uint8_t *buf, uint32_t len) {
    i2c_writeto(imu.i2c_id, imu.i2c_addr, &reg, 1, true);
    i2c_readfrom(imu.i2c_id, imu.i2c_addr, buf, len);
}

/* Little-endian 16-bit sample at buf[0..1]. */
static int16_t sample_le16(const uint8_t *buf) {
    return (int16_t) ((uint16_t) buf[0] | ((uint16_t) buf[1] << 8));
}

bool imu_init(void) {
    /* board_init() already did this, but re-applying makes the component
     * usable standalone and is idempotent. */
    i2c_gpio_init(imu.scl, imu.sda);
    i2c_init(imu.i2c_id);

    reg_write(REG_CTRL1_XL, CFG_CTRL1_XL);
    reg_write(REG_CTRL2_G, CFG_CTRL2_G);

    return imu_who_am_i() == IMU_WHO_AM_I_EXPECT;
}

uint8_t imu_who_am_i(void) {
    return reg_read(REG_WHO_AM_I);
}

void imu_read_accel(imu_sample_t *accel) {
    imu_read(accel, NULL);
}

void imu_read_gyro(imu_sample_t *gyro) {
    imu_read(NULL, gyro);
}

void imu_read(imu_sample_t *accel, imu_sample_t *gyro) {
    /* gyro x/y/z (0..5) then accel x/y/z (6..11), auto-incremented. */
    uint8_t raw[12];
    reg_read_burst(REG_OUTX_L_G, raw, sizeof raw);

    if (gyro) {
        gyro->x = sample_le16(&raw[0]);
        gyro->y = sample_le16(&raw[2]);
        gyro->z = sample_le16(&raw[4]);
    }
    if (accel) {
        accel->x = sample_le16(&raw[6]);
        accel->y = sample_le16(&raw[8]);
        accel->z = sample_le16(&raw[10]);
    }
}
