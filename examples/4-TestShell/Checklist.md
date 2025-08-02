# GPIO HAL Testing Checklist

## Initialization Functions

### `gpio_set_mode(gpio_id_t, moder_t)`
- [ ] Set pin to `MODER_INPUT`
- [x] Set pin to `MODER_OUTPUT`
- [ ] Set pin to `MODER_AF`
- [ ] Set pin to `MODER_ANALOG`
- [ ] Verify mode changes via `gpio_get_mode()`

### `gpio_set_output_type(gpio_id_t, otype_t)`
- [ ] Set pin to `PUSH_PULL`
- [ ] Set pin to `OPEN_DRAIN`

### `gpio_set_output_speed(gpio_id_t, ospeed_t)`
- [ ] Set pin to `LOW_S`
- [ ] Set pin to `MEDIUM_S`
- [ ] Set pin to `HIGH_S`
- [ ] Set pin to `VERY_HIGH_S`

### `gpio_set_pupd(gpio_id_t, pupdr_t)`
- [ ] Set pin to `NONE`
- [ ] Set pin to `PULL_UP`
- [ ] Set pin to `PULL_DOWN`

### `gpio_set_alternate_function(gpio_id_t, afr_t)`
- [ ] Set AF to `AF0` through `AF15`
- [ ] Verify AF registers are updated correctly

### `gpio_set_analog_switch(gpio_id_t)`
- [ ] Enable analog switch and verify behavior if testable

---

## ⚙️ Processing Functions

### `gpio_read(gpio_id_t)`
- [ ] Read LOW from input pin
- [ ] Read HIGH from input pin
- [ ] Test on known state (external connection or internal pull)

### `gpio_write(gpio_id_t, sig_t)`
- [ ] Write LOW
- [ ] Write HIGH
- [ ] Read back output to verify

### `gpio_toggle(gpio_id_t)`
- [x] Toggle

---

## 🚨 Interrupt Functions

### `gpio_enable_interrupt(gpio_id_t, edge_t)`
- [ ] Enable RISING_EDGE interrupt
- [ ] Enable FALLING_EDGE interrupt
- [ ] Verify interrupt triggers on correct edge

### `gpio_disable_interrupt(gpio_id_t, edge_t)`
- [ ] Disable previously enabled RISING_EDGE interrupt
- [ ] Disable previously enabled FALLING_EDGE interrupt
- [ ] Verify interrupt no longer triggers

### `gpio_interrupt_register_handler(gpio_id_t, callbackfn_t)`
- [ ] Register handler and verify it's called
- [ ] Test with multiple pins and handlers
- [ ] Ensure handler is not called after `gpio_disable_interrupt`

---

## 🧪 Test Helpers

### `gpio_get_mode(gpio_id_t)`
- [ ] Verify it reflects mode set by `gpio_set_mode`

### `gpio_get_port_from_portpin(gpio_id_t)`
- [ ] Test with various `gpio_id_t` values (e.g., A5, B12, C3)
- [ ] Verify correct port is extracted
