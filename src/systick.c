/*********************************************************************/
/* SysTick Timer Hardware Abstraction layer*/
/*********************************************************************/
#include "libstefi/systick.h"

#include <gpio_internal.h>

#include "internal/systick_internal.h"

volatile uint32_t ms_counter = 0; //2^32 => Overflow in 49 days

void systick_init()
{
    SysTick->CTRL |= (1 << 2); //1: Clock 0: Clock/8.
    SysTick->CTRL |= (1 << 1); //enable interrupt

    SysTick->LOAD = TICKS_PER_MS ; //System Core Clock=16MHz
    SysTick->VAL   = 0;
}

void systick_start()
{
    ms_counter = 0;
    SysTick->CTRL |= (1 << 0); //start timer
}

void systick_stop()
{
    SysTick->CTRL &= ~(1 << 0); //stop timer
    SysTick->VAL = 0; //Manual reset
}

void systick_restart()
{
    systick_stop();
    systick_start();
}

uint32_t systick_get_ms()
{
    return ms_counter;
}

uint32_t systick_get_us()
{
    return SysTick->VAL;
}

void systick_delay_ms(uint32_t delay)
{
    uint32_t current = ms_counter;
    while((systick_get_ms() - current) <  delay){}
}

bool systick_expired(uint32_t *current, uint32_t period) {
    uint32_t now = systick_get_ms();
    if((now - *current) >= period) {
        *current = now;
        return true;
    }
    return false;
}

/*********IRQ HANDLER ***************/
void SysTick_IRQHandler(void) {
    ms_counter++;
    //generic dispatch (test read temp every  second)
}