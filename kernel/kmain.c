#include <hal/cpu.h>
#include <boot/boot_info.h>
#include <core/console.h>

void kmain(struct sysitn_boot_info *boot_info) {
    console_init(boot_info->framebuffer);

    kprintf("SysitnOS kernel started!\n");
    kprintf("Framebuffer: %s\n", boot_info->framebuffer ? "available" : "not available");

    while (1) {
        cpu_halt();
    }
}