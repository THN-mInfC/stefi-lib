#include "board.h"
#include "led.h"
#include "motor.h"
#include "line_sensor.h"
#include "imu.h"

#include <libstefi/systick.h>
#include <stdio.h>

/*
 * Rohmi (NUCLEO-G431KB) board smoke test.
 *
 * LSM6DSO IMU on I2C2 via the imu component: prints WHO_AM_I (must be 0x6C),
 * then streams accel [mg] and gyro [mdps] at 2 Hz on the VCP (115200 baud).
 * Board flat on the desk: expect z = +1000 mg, x = y = 0, gyro = 0 (small
 * offsets are normal). The I2C driver blocks without timeout on NACK: if the
 * output stops at "probing", the address (SA0 strap) or the wiring is wrong.
 *
 * Also exercises the motor + line_sensor components: every loop also prints the
 * line-sensor state. The motor self-test is OFF by default so the car does not
 * drive on power-up — put it on blocks, then set MOTOR_SELFTEST to 1 to verify
 * direction (MOTOR_FORWARD_LEVEL) and duty.
 */
#define MOTOR_SELFTEST 0

static const char *line_state_name(line_state_t s) {
    switch (s) {
        case LINE_STATE_LOST:  return "LOST";
        case LINE_STATE_LEFT:  return "LEFT";
        case LINE_STATE_RIGHT: return "RIGHT";
        case LINE_STATE_BOTH:  return "BOTH";
        default:               return "?";
    }
}

#if MOTOR_SELFTEST
static void motor_selftest(void) {
    const int8_t speed = 30;            //gentle
    printf("motor self-test: L+R forward \r\n");
    motor_drive(MOTOR_L, speed);
    motor_drive(MOTOR_R, speed);
    systick_delay_ms(1000);

    printf("motor self-test: L+R reverse \r\n");
    motor_drive(MOTOR_L, -speed);
    motor_drive(MOTOR_R, -speed);
    systick_delay_ms(1000);

    motor_stop_all();
    printf("motor self-test: done \r\n");
}
#endif

void main(void) {
    board_init();

    printf("\r\nRohmi board test: LSM6DSO IMU on I2C2 \r\n");
    printf("probing addr 0x%02X \r\n", imu.i2c_addr);

    bool ok = imu_init();
    printf("WHO_AM_I = 0x%02X (expect 0x%02X) -> %s \r\n",
           imu_who_am_i(), IMU_WHO_AM_I_EXPECT, ok ? "OK" : "FAIL");

#if MOTOR_SELFTEST
    motor_selftest();
#else
    motor_stop_all();
#endif

    while (1) {
        led_toggle(LED_NUCLEO);

        printf("line: L=%d R=%d  state=%s \r\n",
               line_sensor_on_line(LINE_SENSOR_L),
               line_sensor_on_line(LINE_SENSOR_R),
               line_state_name(line_sensor_state()));

        imu_sample_t a, g;
        imu_read(&a, &g);
        printf("acc[mg] x=%5ld y=%5ld z=%5ld | gyro[mdps] x=%7ld y=%7ld z=%7ld \r\n",
               (long) imu_accel_mg(a.x), (long) imu_accel_mg(a.y), (long) imu_accel_mg(a.z),
               (long) imu_gyro_mdps(g.x), (long) imu_gyro_mdps(g.y), (long) imu_gyro_mdps(g.z));

        systick_delay_ms(500);
    }
}
