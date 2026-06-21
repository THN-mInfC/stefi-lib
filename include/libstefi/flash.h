#pragma once

#include <stdbool.h>
#include <stdint.h>

/*
 * Internal-flash erase/program for the STM32G431 (single bank, 2 KB pages).
 *
 * Flash is memory-mapped for reads: read back simply by dereferencing the
 * address (e.g. flash_page_addr(page)). Writes require the page to be erased
 * first, and are programmed a double word (8 bytes) at a time, so addr and len
 * passed to flash_write() must both be 8-byte aligned.
 *
 * Unlock/lock are handled internally by erase/write. Erasing or programming a
 * page that holds executing code will hang/fault — keep app data pages clear of
 * the .text region (the map uses the last page; see map_storage in the app).
 */

#define FLASH_PAGE_SIZE 2048u
#define FLASH_NUM_PAGES 64u             /* G431KB: 128 KB / 2 KB */
#define FLASH_MEM_BASE  0x08000000u

/* Absolute address of a page (page 0..FLASH_NUM_PAGES-1). */
static inline uint32_t flash_page_addr(uint32_t page) {
    return FLASH_MEM_BASE + page * FLASH_PAGE_SIZE;
}

/* Erase one page. Returns false on a bad page index or a flash error. */
bool flash_erase_page(uint32_t page);

/*
 * Program len bytes from data to flash address addr. addr and len must be
 * multiples of 8 (double-word programming). The target must already be erased.
 * Returns false on misalignment or a flash error.
 */
bool flash_write(uint32_t addr, const void *data, uint32_t len);
