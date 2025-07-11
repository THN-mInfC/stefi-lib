#pragma once
/*********************************************************************/
/* SysTick Timer Hardware Abstraction layer*/
/*********************************************************************/
#include <stdbool.h>
#include <stdint.h>

//Systick generates Interrupt every 1 ms
void systick_init();
void systick_start();
void systick_stop();
//reset the timer
void systick_restart();

uint32_t systick_get_ms();
uint32_t systick_get_us();

//Non-blocking delay
bool systick_expired(uint32_t *current, uint32_t period);
//Blocking delay
void systick_delay_ms(uint32_t time_in_ms);