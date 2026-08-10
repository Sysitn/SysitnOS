#include <hal/cpu.h>
#include <stdint.h>
#include <stddef.h>
#include <boot/boot_info.h>

void kmain(struct sysitn_boot_info *boot_info) {

    uint32_t *fb_addr = (uint32_t *) boot_info->framebuffer->address;

    for (int i = 0; i < 10000; i++) {
        fb_addr[i] = 0x0000FF00;
    }

    while (1) {
        cpu_halt();
    }
}