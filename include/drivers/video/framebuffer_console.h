#pragma once
#include <stddef.h>
#include <boot/boot_info.h>

void framebuffer_init(struct sysitn_framebuffer *fb);
void framebuffer_write(const char *str, size_t len);