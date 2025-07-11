#include "libstefi/rng.h"
#include "libstefi/peripheral.h"

#include <../src/internal/rng_internal.h>
#include <libstefi/util.h>

void rng_init() {
    //initialize rcc clock
    peripheral_rng_enable();
    // Enable RNG
    RNG->CR |= BIT(2);
}

uint32_t rng_get_rand() {
    // Wait for data ready, Error handling later
    while ((RNG->SR & BIT(0)) == 0);

    // Return the random number
    return RNG->DR;
}