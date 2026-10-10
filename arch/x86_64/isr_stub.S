.intel_syntax noprefix

.macro ISR_NOERRCODE num
    .global isr\num
    isr\num:
        push 0
        push \num
        jmp isr_common_stub
.endm

.macro ISR_ERRCODE num
    .global isr\num
    isr\num:
        push \num
        jmp isr_common_stub
.endm

ISR_NOERRCODE 0   /* #DE Division Error */
ISR_NOERRCODE 1   /* #DB Debug */
ISR_NOERRCODE 2   /* NMI Interrupt */
ISR_NOERRCODE 3   /* #BP Breakpoint */
ISR_NOERRCODE 4   /* #OF Overflow */
ISR_NOERRCODE 5   /* #BR BOUND Range Exceeded */
ISR_NOERRCODE 6   /* #UD Invalid Opcode */
ISR_NOERRCODE 7   /* #NM Device Not Available */
ISR_ERRCODE   8   /* #DF Double Fault */
ISR_NOERRCODE 9   /* Coprocessor Segment Overrun */
ISR_ERRCODE   10  /* #TS Invalid TSS */
ISR_ERRCODE   11  /* #NP Segment Not Present */
ISR_ERRCODE   12  /* #SS Stack Segment Fault */
ISR_ERRCODE   13  /* #GP General Protection Fault */
ISR_ERRCODE   14  /* #PF Page Fault */
ISR_NOERRCODE 15  /* Reserved */
ISR_NOERRCODE 16  /* #MF x87 FPU Floating-Point Error */
ISR_ERRCODE   17  /* #AC Alignment Check */
ISR_NOERRCODE 18  /* #MC Machine Check */
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

ISR_NOERRCODE 32 /* Timer */
ISR_NOERRCODE 33 /* Keyboard */
ISR_NOERRCODE 34
ISR_NOERRCODE 35
ISR_NOERRCODE 36
ISR_NOERRCODE 37
ISR_NOERRCODE 38
ISR_NOERRCODE 39
ISR_NOERRCODE 40
ISR_NOERRCODE 41
ISR_NOERRCODE 42
ISR_NOERRCODE 43
ISR_NOERRCODE 44
ISR_NOERRCODE 45
ISR_NOERRCODE 46
ISR_NOERRCODE 47

.extern isr_handler

isr_common_stub:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, rsp
    call isr_handler

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax

    add rsp, 16

    iretq