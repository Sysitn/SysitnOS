#include <core/console.h>
#include <drivers/video/framebuffer_console.h>
#include <drivers/uart.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_CONSOLES 4
#define KPRINTF_BUFFER_SIZE 512

struct sink {
    char *buf;
    size_t size;
    size_t written;
    size_t requested;
};

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

static inline void sink_putc(struct sink *s, char c) {
    s->requested++;
    if (s->written + 1 < s->size) {
        s->buf[s->written++] = c;
    }
}

static void print_num(struct sink *s, uint64_t uval, int base) {

}

static int kvsnprintf(char *buffer, size_t size, const char *fmt, va_list args) {
    struct sink s = {
        .buf = buffer,
        .size = size,
        .written = 0,
        .requested = 0
    };

    while (*fmt != '\0') {
        if (*fmt != '%') {
            sink_putc(&s, *fmt++);
            continue;
        }

        fmt++;

        if (*fmt == '\0') {
            break;
        }

        switch (*fmt) {
            case 'c': {
                char c = (char) va_arg(args, int);
                sink_putc(&s, c);
                break;
            }
            case 's': {
                const char *str = va_arg(args, const char *);
                if (!str) {
                    str = "(null)";
                }
                while (*str) {
                    sink_putc(&s, *str++);
                }
                break;
            }
            case 'd': {
                break;
            }
            case '%': {
                sink_putc(&s, '%');
                break;
            }
        }
        fmt++;
    }

    if (s.size > 0) {
        buffer[s.written < s.size ? s.written : s.size - 1] = '\0';
    }

    return (int) s.requested;
}

int kprintf(const char *fmt, ...) {
    char buffer[KPRINTF_BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    int len = kvsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    size_t real_len = (len < (int) sizeof(buffer)) ? (size_t) len : sizeof(buffer) - 1;

    for (int i = 0; i < consoles_count; i++) {
        consoles[i](buffer, real_len);
    }

    return len;
}