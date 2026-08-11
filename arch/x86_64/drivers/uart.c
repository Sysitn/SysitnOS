#include <drivers/uart.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define COM1_BASE 0x3F8

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("in %0, %1" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("out %1, %0" : : "a"(val), "Nd"(port));
}

void uart_write(const char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        while ((inb(COM1_BASE + 5)& 0x20) == 0);
        outb(COM1_BASE, str[i]);
    }
}

bool uart_available(void) {
    outb(COM1_BASE + 7, 0xAE);
    if (inb(COM1_BASE + 7) != 0xAE) {
        return false;
    }
    return true;
}