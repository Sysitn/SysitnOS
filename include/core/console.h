#pragma once
#include <stddef.h>
#include <boot/boot_info.h>

#define KPRINTF_INFO " [ INFO ] "
#define KPRINTF_OK " [ OK ] "
#define KPRINTF_WARNING " [ WARNING ] "
#define KPRINTF_ERROR " [ ERROR ] "

typedef void (*console_write_fun) (const char *str, size_t len);

void console_init(struct sysitn_framebuffer *framebuffer);
void console_register(console_write_fun fun);
int kprintf(const char *fmt, ...);