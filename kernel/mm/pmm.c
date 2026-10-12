#include <stdint.h>
#include <stdbool.h>
#include <core/pmm.h>
#include <core/console.h>

#define PAGE_SIZE 4096

static uint8_t *bitmap;
static size_t total_pages;
static size_t hint;
static size_t count_free_pages;

static inline void bit_set(uint64_t i) { 
    bitmap[i / 8] |= (1u << (i % 8));
}

static inline void bit_clear(uint64_t i) {
    bitmap[i / 8] &= ~(1u << (i % 8));
}

// Если бит занят, то вернуть 1, в другом случае - 0
static inline bool bit_test(uint64_t i) {
    return bitmap[i / 8] & (1u << (i % 8));
}

void pmm_init(const struct sysitn_memmap *memmap, uintptr_t hhdm_offset) {
    if (!memmap) {
        kprintf(KPRINTF_ERROR "Physical Memory Manager Not Started\n");
        return;
    }

    phys_addr_t max_addr = 0;

    kprintf(KPRINTF_INFO "memmap regions: %d\n", (int) memmap->count_entries);
    for (size_t i = 0; i < memmap->count_entries; i++) {
        kprintf(KPRINTF_WARNING "\n");
    }

    total_pages = max_addr / PAGE_SIZE;

    kprintf(KPRINTF_OK " Physical Memory Manager Initialized\n");
    kprintf(KPRINTF_INFO "PMM Total Pages: %d\n", total_pages);
}

phys_addr_t pmm_alloc_page(void) {
    for (uint64_t n = 0; n < total_pages; n++) {
        size_t i = (hint + n) % total_pages;
        if (!bit_test(n)) {
            bit_set(i);
            hint = i + 1;
            count_free_pages--;
            return (phys_addr_t) i * PAGE_SIZE;
        }
    }
    return 0;
}

void pmm_free_page(phys_addr_t phys) {
    size_t i = phys / PAGE_SIZE;
    if (!bit_test(i)) {
        return;
    }
    bit_clear(i);
    count_free_pages++;
    if (i < hint) {
        hint = i;
    }
}

uint64_t pmm_count_free_pages(void) {
    return count_free_pages;
}