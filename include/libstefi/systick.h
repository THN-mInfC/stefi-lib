#pragma once
/*********************************************************************/
/* SysTick Timer Hardware Abstraction layer*/
/*********************************************************************/
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initialize SysTick to trigger interrupt every 1 ms
 */
void systick_init(void);

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