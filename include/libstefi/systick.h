#pragma once
/*********************************************************************/
/* SysTick Timer Hardware Abstraction layer*/
/*********************************************************************/
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initialize SysTick to trigger interrupt every 1 ms
 * @param sysclk_hz Current system clock in Hz. The boards pass their boot
 *                  clock (BOARD_SYSCLK: 4 MHz MSI on the L476, 16 MHz HSI16
 *                  on the G431). After reconfiguring the clock (e.g.
 *                  system_init() to 80 MHz), call again with the new value.
 */
void systick_init(uint32_t sysclk_hz);

/**
 * @brief Start the SysTick timer
 */
void systick_start(void);

/**
 * @brief Stop the SysTick timer
 */
void systick_stop(void);

/**
 * @brief Restart SysTick timer (reset to 0 and start)
 */
void systick_restart(void);

/**
 * @brief Get milliseconds since system start
 * @return Elapsed time in milliseconds
 */
uint32_t systick_get_ms(void);

/**
 * @brief Get microseconds precision to millis counter
 */
uint32_t systick_get_us(void);

/**
 * @brief For non-blocking delay, Check if the given period has elapsed since last call
 *
 * @param last_timestamp Pointer to store and compare previous tick
 * @param period_ms Period duration in milliseconds
 * @return true if period has elapsed
 */
bool systick_expired(uint32_t *current, uint32_t period);

/**
 * @brief Blocking delay
 */
void systick_delay_ms(uint32_t time_in_ms);