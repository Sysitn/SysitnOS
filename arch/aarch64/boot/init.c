#include <boot/boot_info.h>

static struct sysitn_boot_info boot_info;
static struct sysitn_framebuffer framebuffer;
static struct sysitn_memmap memmap;

void _boot(uint64_t dtb_ptr) {

    

    kmain(&boot_info);

    __asm__ volatile ("wfi");
}