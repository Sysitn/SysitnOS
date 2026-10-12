#include <drivers/uart.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <arch/x86_64/io.h>

#define COM1 0x3F8

void uart_init(void) {
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x80);    
    outb(COM1 + 0, 0x01);    
    outb(COM1 + 1, 0x00);    
    outb(COM1 + 3, 0x03);    
    outb(COM1 + 2, 0xC7);    
    outb(COM1 + 4, 0x0B);
}

void uart_write(const char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        while ((inb(COM1 + 5)& 0x20) == 0);
        outb(COM1, str[i]);
    }
}

bool uart_available(void) {
    outb(COM1 + 7, 0xAE);
    if (inb(COM1 + 7) != 0xAE) {
        return false;
    }
    return true;
}