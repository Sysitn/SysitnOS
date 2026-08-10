#pragma once

#include <stdint.h>
#include <stddef.h>

struct sysitn_framebuffer {
    void* address;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint16_t bpp;
};

struct sysitn_boot_info {
    struct sysitn_framebuffer *framebuffer;
    uint64_t hhdm_offset;
};

void kmain(struct sysitn_boot_info *boot_info);