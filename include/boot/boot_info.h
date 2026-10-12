#pragma once

#include <stdint.h>
#include <stddef.h>

typedef uint64_t phys_addr_t;

struct sysitn_memmap_entry {
    phys_addr_t base;
    uint64_t len;
};

struct sysitn_memmap {
    uint64_t count_entries;
    struct sysitn_memmap_entry **memmap_entries;
};

struct sysitn_framebuffer {
    void* address;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint16_t bpp;
};

struct sysitn_boot_info {
    struct sysitn_framebuffer *framebuffer;
    struct sysitn_memmap *memmap;
    uint64_t hhdm_offset;
};

void kmain(struct sysitn_boot_info *boot_info);