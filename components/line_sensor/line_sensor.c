#include "line_sensor.h"

#include "libstefi/peripheral.h"
#include "libstefi/gpio.h"

/*
 * GPIO level the reflectance module outputs when it is over the track line.
 * Verified on the RobOhmobil: the line is BLACK and the modules drive the pin
 * HIGH over it (LOW over the white floor). Flip if line_sensor_on_line() reads
 * inverted on a different module / contrast.
 */
#define LINE_DETECTED_LEVEL HIGH

void line_sensor_init(void) {
    for (uint32_t i = 0; i < NUM_LINE_SENSORS; i++) {
        gpio_id_t pin = line_sensors[i].portpin;
        peripheral_gpio_enable(gpio_get_port_from_portpin(pin));
        gpio_set_mode(pin, MODER_INPUT);
    }
}

bool line_sensor_read(stefi_line_sensor_t id) {
    return gpio_read(line_sensors[id].portpin) == HIGH;
}

bool line_sensor_on_line(stefi_line_sensor_t id) {
    return gpio_read(line_sensors[id].portpin) == LINE_DETECTED_LEVEL;
}

line_state_t line_sensor_state(void) {
    bool left = line_sensor_on_line(LINE_SENSOR_L);
    bool right = line_sensor_on_line(LINE_SENSOR_R);

    if (left && right) {
        return LINE_STATE_BOTH;
    }
    if (left) {
        return LINE_STATE_LEFT;
    }
    if (right) {
        return LINE_STATE_RIGHT;
    }
    return LINE_STATE_LOST;
}
