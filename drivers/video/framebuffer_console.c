#include <drivers/video/framebuffer_console.h>
#include <stdint.h>

static uint32_t console_x = 0;
static uint32_t console_y = 0;

void framebuffer_init(void *addr, uint32_t width, uint32_t height, uint32_t pitch, uint16_t bpp) {
    
}

void framebuffer_write(const char *str, size_t len) {

}