#pragma once
#include <stddef.h>

void framebuffer_init(void *addr, uint32_t width, uint32_t height, uint32_t pitch, uint16_t bpp);
void framebuffer_write(const char *str, size_t len);