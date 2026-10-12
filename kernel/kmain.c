#include <hal/cpu.h>
#include <boot/boot_info.h>
#include <core/pmm.h>
#include <core/console.h>

void kmain(struct sysitn_boot_info *boot_info) {
    console_init(boot_info->framebuffer);
    cpu_init();
    pmm_init(boot_info->memmap, boot_info->hhdm_offset);

    int d = 10;

    kprintf("Zero: %p", &d);

    while (1) {
        cpu_halt();
    }
}