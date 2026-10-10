#include <hal/cpu.h>
#include <arch/x86_64/idt.h>
#include <arch/x86_64/pic.h>
#include <core/console.h>

void cpu_init(void) {
    __asm__ volatile ("cli");
    pic_init();
    idt_init();
    __asm__ volatile ("sti");
    kprintf("[ INFO ] CPU Interrupt started\n");
}

void cpu_halt(void) {
    __asm__ volatile ("hlt");
}