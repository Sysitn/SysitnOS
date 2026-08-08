#include <hal/cpu.h>

void _start(void) {

    while (1) {
        cpu_halt();
    }
}