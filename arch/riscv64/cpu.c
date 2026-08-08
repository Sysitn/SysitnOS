#include <hal/cpu.h>

void cpu_init(void) {

}

void cpu_halt(void) {
    __asm__ volatile ("wfi");
}