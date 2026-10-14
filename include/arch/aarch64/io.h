#pragma once
#include <stdint.h>

static inline uint8_t inb(uint64_t port) {
    return port;
}

static inline void outb(uint64_t port, uint8_t value) {
    port = value;
}