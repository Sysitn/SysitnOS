
.global _start
.section .text.boot

.extern _boot

_start:
    ldr x1, =stack_top
    mov sp, x1

    bl _boot

.halt:
    wfi
    b .halt

.section .bss
.align 16
stack_bottom:
    .space 16384
stack_top: