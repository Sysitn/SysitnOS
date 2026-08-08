#include <hal/cpu.h>

void cpu_init(void) {
    // TODO
}

void cpu_halt(void) {
    __asm__ volatile ("hlt");
}