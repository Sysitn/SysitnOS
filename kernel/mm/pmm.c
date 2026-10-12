#include <stdint.h>
#include <stdbool.h>
#include <core/pmm.h>
#include <core/console.h>

#define PAGE_SIZE 4096

static uint8_t *bitmap;
static size_t bitmap_bytes;
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
    if (!memmap || !hhdm_offset) {
        kprintf(KPRINTF_ERROR "Physical Memory Manager Not Started\n");
        return;
    }

    phys_addr_t max_addr = 0;

    for (size_t i = 0; i < memmap->count_entries; i++) {
        phys_addr_t end = memmap->memmap_entries[i].base + memmap->memmap_entries[i].len;
        if (end > max_addr) {
            max_addr = end;
        }
    }

    total_pages = max_addr / PAGE_SIZE;

    bitmap_bytes = (total_pages + 7) / 8;

    phys_addr_t bitmap_phys = 0;
    for (size_t i = 0; i < memmap->count_entries; i++) {
        if (memmap->memmap_entries[i].len >= bitmap_bytes) {
            bitmap_phys = memmap->memmap_entries[i].base;
            break;
        }
    }

    if (bitmap_phys == 0) {
        kprintf(KPRINTF_ERROR "Physical Manager Not Started: insufficient memory for bitmap\n");
        return;
    }

    bitmap = (uint8_t*) (bitmap_phys + hhdm_offset);

    for (size_t i = 0; i < bitmap_bytes; i++) {
        bitmap[i] = 0xFF;
    }

    for (size_t i = 0; i < memmap->count_entries; i++) {
        size_t first = memmap->memmap_entries[i].base / PAGE_SIZE;
        size_t last  = (memmap->memmap_entries[i].base + memmap->memmap_entries[i].len) / PAGE_SIZE;
        for (size_t p = first; p < last; p++) {
            bit_clear(p);
        }
    }

    size_t bitmap_pages = (bitmap_bytes + PAGE_SIZE - 1) / PAGE_SIZE;
    for (size_t p = 0; p < bitmap_pages; p++) {
        bit_set(bitmap_phys / PAGE_SIZE + p);
    }

    bit_set(0);

    for (size_t i = 0; i < total_pages; i++) {
        if (!bit_test(i)) {
            count_free_pages++;
        }
    }

    hint = 0;

    kprintf(KPRINTF_OK " Physical Memory Manager Initialized\n");
    kprintf(KPRINTF_INFO "PMM Total Pages: %d\n", total_pages);
    kprintf(KPRINTF_INFO "Free pages: %d\n", pmm_count_free_pages());
    kprintf(KPRINTF_INFO "Bitmap pointer: %p\n", bitmap);
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