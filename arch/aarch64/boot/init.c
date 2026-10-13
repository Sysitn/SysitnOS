#include <boot/boot_info.h>

static struct sysitn_boot_info boot_info;

void _boot(uint64_t dtb_ptr) {

    boot_info.framebuffer = NULL;
    boot_info.hhdm_offset = 0;
    boot_info.memmap = NULL;

    kmain(&boot_info);

    __asm__ volatile ("wfi");
}