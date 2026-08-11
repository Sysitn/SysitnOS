#include <core/console.h>
#include <drivers/video/framebuffer_console.h>
#include <drivers/uart.h>
#include <stdarg.h>

#define MAX_CONSOLES 4
#define BUFFER_SIZE 256

static console_write_fun consoles[MAX_CONSOLES];
static int consoles_count = 0;

void console_init(struct sysitn_framebuffer *framebuffer) {
    if (uart_available()) {
        console_register(uart_write);
    }
    if (framebuffer != NULL) {
        console_register(framebuffer_write);
    }
}

void console_register(console_write_fun fun) {
    if (consoles_count < MAX_CONSOLES) {
        consoles[consoles_count++] = fun;
    }
}

void kprintf(const char *fmt, ...) {
    char buffer[BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    size_t len = 45;
    va_end(args);
    for (int i = 0; i < consoles_count; i++) {
        consoles[i](buffer, len);
    }
}