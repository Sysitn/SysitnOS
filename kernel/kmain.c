#include <hal/cpu.h>
#include <boot/boot_info.h>
#include <core/pmm.h>
#include <core/console.h>

void kmain(struct sysitn_boot_info *boot_info) {
    console_init(boot_info->framebuffer);
    cpu_init();
    pmm_init();

    kprintf("Count PMM free pages: %d\n", (int) pmm_count_free_pages());

    while (1) {
        cpu_halt();
    }
}