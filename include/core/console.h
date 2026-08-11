#pragma once
#include <stddef.h>
#include <boot/boot_info.h>

typedef void (*console_write_fun) (const char *str, size_t len);

void console_init(struct sysitn_framebuffer *framebuffer);
void console_register(console_write_fun fun);
void kprintf(const char *fmt, ...);