#include <arch/x86_64/idt.h>

__attribute__((aligned(0x10)))
static struct idt_entry idt[256];
static struct idt_ptr idtr;

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr13(void);
extern void isr14(void);
extern void isr32(void);
extern void isr33(void);

void idt_load(void) {
    __asm__ volatile ("lidt %0" :: "m" (idtr));
}

void idt_init(void) {
    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint64_t) &idt;

    for (int i = 0; i < 256; i++) {
        set_idt_entry(i, 0, 0, 0x8E);
    }

    set_idt_entry(0, (uint64_t *) isr0, 0, 0x8E);
    set_idt_entry(1, (uint64_t *) isr1, 0, 0x8E);
    set_idt_entry(2, (uint64_t *) isr2, 0, 0x8E);
    set_idt_entry(3, (uint64_t *) isr3, 0, 0x8E);
    set_idt_entry(13, (uint64_t *) isr13, 0, 0x8E);
    set_idt_entry(14, (uint64_t *) isr14, 0, 0x8E);
    set_idt_entry(32, (uint64_t *) isr32, 0, 0x8E);
    set_idt_entry(33, (uint64_t *) isr33, 0, 0x8E);

    idt_load();
}

void set_idt_entry(uint8_t vector, void *isr, uint8_t ist, uint8_t attributes) {
    uint64_t addr = (uint64_t) isr;
    idt[vector].isr_low = addr & 0xFFFF;
    idt[vector].attributes = attributes;
    idt[vector].kernel_cs = 0x28;
    idt[vector].ist = ist;
    idt[vector].isr_mid = (addr >> 16) & 0xFFFF;
    idt[vector].isr_high = (addr >> 32) & 0xFFFFFFFF;
    idt[vector].reserved = 0;
}