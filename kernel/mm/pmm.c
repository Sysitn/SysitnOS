#include <stdint.h>
#include <stdbool.h>
#include <core/console.h>

#define PAGE_SIZE 4096

static uint8_t *bitmap;
static size_t total_pages;
static size_t hint;
static size_t count_free_pages;

static inline void bit_set(uint64_t i) { 
    bitmap[i / 8] |= (1 << (i % 8));
}

static inline void bit_clear(uint64_t i) {
    bitmap[i / 8] &= ~(1 << (i % 8));
}

// Если бит занят, то вернуть 1, в другом случае - 0
static inline bool bit_test(uint64_t i) {
    return bitmap[i / 8] & (1 << (i % 8));
}

void pmm_init() {
    kprintf("[ INFO ] Physical Memory Manager Initialized\n");
}

uint64_t pmm_alloc_page(void) {
    for (uint64_t n = 0; n < total_pages; n++) {
        if (!bit_test(n)) {
            
        }
    }
}

void pmm_free_page(uint64_t phys) {
    bit_clear(phys / PAGE_SIZE);
}

uint64_t pmm_count_free_pages(void) {
    return count_free_pages;
}