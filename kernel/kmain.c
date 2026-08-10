#include <hal/cpu.h>
#include <stdint.h>
#include <stddef.h>
#include <boot/boot_info.h>

void kmain(struct sysitn_boot_info *boot_info) {

    

    while (1) {
        cpu_halt();
    }
}