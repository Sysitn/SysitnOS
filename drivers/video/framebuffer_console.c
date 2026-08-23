#include <drivers/video/framebuffer_console.h>
#include <boot/boot_info.h>
#include <fonts/font8x16.h>
#include <stdint.h>

static uint32_t cursor_x = 0;
static uint32_t cursor_y = 0;

static uint32_t *addr;
static uint32_t width;
static uint32_t height;
static uint32_t pitch_in_pixels;

void framebuffer_init(struct sysitn_framebuffer *fb) {
    addr = (uint32_t *) fb->address;
    width = fb->width;
    height = fb->height;
    pitch_in_pixels = fb->pitch / 4;
}

void framebuffer_putc(const char c) {
    if (cursor_x + 8 > width) {
        cursor_x = 0;
        cursor_y += 16;
    }

    if (c == '\n') {
        cursor_x = 0;
        cursor_y += 16;
        return;
    }

    if (cursor_y + 8 > height) {
        return;
    }

    const uint8_t *symbol = font8x16[(unsigned char) c];

    for (size_t y = 0; y < 16; y++) {
        uint8_t line = symbol[y];
        for (size_t x = 0; x < 8; x++) {
            if (line & (1 << (7 - x))) {
                addr[(cursor_y + y) * pitch_in_pixels + (cursor_x + x)] = 0x00FFFFFF;
            } else {
                addr[(cursor_y + y) * pitch_in_pixels + (cursor_x + x)] = 0x00000000;
            }
        }
    }

    cursor_x += 8;
}

void framebuffer_write(const char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        framebuffer_putc(*str++);
    }
}
