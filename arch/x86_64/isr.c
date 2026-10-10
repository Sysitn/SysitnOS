#include <arch/x86_64/idt.h>
#include <core/console.h>
#include <arch/x86_64/io.h>

const char *exception_messages[] = {
    "Division By Zero", "Debug", "Non Maskable Interrupt", "Breakpoint",
    "Into Detected Overflow", "Out of Bounds", "Invalid Opcode", "No Coprocessor",
    "Double Fault", "Coprocessor Segment Overrun", "Bad TSS", "Segment Not Present",
    "Stack Fault", "General Protection Fault", "Page Fault", "Unknown Interrupt"
};

void isr_handler(registers_t *regs) {
    if (regs->int_no < 32) {
        kprintf("\n[CPU EXCEPTION]: ");
        if (regs->int_no < 16) {
            kprintf(exception_messages[regs->int_no]);
        }
        kprintf("\nKernel panic!");
        
        for(;;) { 
            __asm__ volatile("cli; hlt");
        }
    }

    if (regs->int_no >= 32 && regs->int_no <= 47) {

        if (regs->int_no >= 40) {
            outb(0xA0, 0x20); 
        }
        outb(0x20, 0x20);
    }
}