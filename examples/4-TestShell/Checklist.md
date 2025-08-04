# GPIO HAL Testing Checklist

## Initialization Functions

### `gpio_set_mode(gpio_id_t, moder_t)`
- [x] Set pin to `MODER_INPUT` [gpio_interrupt_test()]
- [x] Set pin to `MODER_OUTPUT` [gpio_toggle_test()]
- [x] Set pin to `MODER_AF` [uart_configure()]
- [ ] Set pin to `MODER_ANALOG` 

### `gpio_set_output_type(gpio_id_t, otype_t)`
- [ ] Set pin to `PUSH_PULL` []
- [ ] Set pin to `OPEN_DRAIN` []

### `gpio_set_output_speed(gpio_id_t, ospeed_t)`
- [ ] Set pin to `LOW_S`, `MEDIUM_S` , `HIGH_S`, `VERY_HIGH_S`

### `gpio_set_pupd(gpio_id_t, pupdr_t)`
- [ ] Set pin to `NONE`, `PULL_DOWN`
- [x] Set pin to `PULL_UP` [gpio_interrupt_test()]

### `gpio_set_alternate_function(gpio_id_t, afr_t)`
- [x] Set AF to `AF0` through `AF15` [uart_configure(), spi_gpio_init(), ...]


### `gpio_set_analog_switch(gpio_id_t)`
- [ ] Enable analog switch and verify behavior if testable


## Processing Functions
### `gpio_write(gpio_id_t, sig_t)` `gpio_read(gpio_id_t)`
- [x] Write LOW, USER LED goes ON [gpio_toggle_test()]
- [x] Read LOW from USER LED pin  [gpio_readwrite_test()]
### `gpio_toggle(gpio_id_t)`
- [x] Toggle

---

## Interrupt Functions

### `gpio_enable_interrupt(gpio_id_t, edge_t)`
- [x] Enable RISING_EDGE interrupt [gpio_interrupt_test()]
- [x] Enable FALLING_EDGE interrupt [gpio_interrupt_test()]

### `gpio_disable_interrupt(gpio_id_t, edge_t)` (not implemented)
- [ ] Verify interrupt no longer triggers

### `gpio_interrupt_register_handler(gpio_id_t, callbackfn_t)`
- [x] Register handler and verify it's called