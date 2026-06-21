#include "libstefi/flash.h"
#include "libstefi/util.h"
#include "internal/flash_internal.h"

#include <string.h>

static void flash_wait_idle(void) {
    while (FLASH_REG->SR & FLASH_SR_BSY) {
    }
}

static void flash_unlock(void) {
    if (FLASH_REG->CR & FLASH_CR_LOCK) {
        FLASH_REG->KEYR = FLASH_KEY1;
        FLASH_REG->KEYR = FLASH_KEY2;
    }
}

static void flash_lock(void) {
    FLASH_REG->CR |= FLASH_CR_LOCK;
}

/* Wait for the current operation, then report and clear EOP + error flags. */
static bool flash_finish(void) {
    flash_wait_idle();
    uint32_t sr = FLASH_REG->SR;
    FLASH_REG->SR = sr & (FLASH_SR_EOP | FLASH_SR_ALL_ERR); /* w1c */
    return (sr & FLASH_SR_ALL_ERR) == 0;
}

bool flash_erase_page(uint32_t page) {
    if (page >= FLASH_NUM_PAGES) {
        return false;
    }

    flash_unlock();
    flash_wait_idle();
    FLASH_REG->SR = FLASH_SR_EOP | FLASH_SR_ALL_ERR; /* clear stale flags */

    uint32_t cr = FLASH_REG->CR & ~FLASH_CR_PNB_MSK;
    cr |= FLASH_CR_PER | (page << FLASH_CR_PNB_POS);
    FLASH_REG->CR = cr;
    FLASH_REG->CR |= FLASH_CR_STRT;

    bool ok = flash_finish();

    FLASH_REG->CR &= ~(FLASH_CR_PER | FLASH_CR_PNB_MSK);
    flash_lock();
    return ok;
}

bool flash_write(uint32_t addr, const void *data, uint32_t len) {
    if ((addr % 8u) != 0 || (len % 8u) != 0) {
        return false;
    }

    const uint8_t *src = (const uint8_t *) data;

    flash_unlock();
    flash_wait_idle();
    FLASH_REG->SR = FLASH_SR_EOP | FLASH_SR_ALL_ERR; /* clear stale flags */
    FLASH_REG->CR |= FLASH_CR_PG;

    bool ok = true;
    for (uint32_t i = 0; i < len && ok; i += 8u) {
        uint32_t w0, w1;
        memcpy(&w0, src + i, 4);
        memcpy(&w1, src + i + 4, 4);
        /* Both words of the double word must be written before BSY clears. */
        *(volatile uint32_t *) (addr + i) = w0;
        *(volatile uint32_t *) (addr + i + 4u) = w1;
        ok = flash_finish();
    }

    FLASH_REG->CR &= ~FLASH_CR_PG;
    flash_lock();
    return ok;
}
