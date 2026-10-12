#include <hal/cpu.h>
#include <boot/boot_info.h>
#include <core/pmm.h>
#include <core/console.h>

extern char __kernel_start;
extern char __kernel_end;

void kmain(struct sysitn_boot_info *boot_info) {
    console_init(boot_info->framebuffer);
    cpu_init();
    pmm_init(boot_info->memmap, boot_info->hhdm_offset);

    size_t kernel_size = (size_t) &__kernel_end - (size_t) &__kernel_start;

    kprintf(KPRINTF_INFO "Kernel size: %d KiB\n", (int) kernel_size / 1024);

    while (1) {
        cpu_halt();
    }
}