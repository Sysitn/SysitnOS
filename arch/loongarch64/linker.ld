OUTPUT_FORMAT(elf64-loongarch)
OUTPUT_ARCH(loongarch64)
ENTRY(_start)

PHDRS 
{
    limine_requests PT_LOAD;
    text   PT_LOAD FLAGS((1 << 0) | (1 << 2));
    rodata PT_LOAD FLAGS((1 << 2));
    data   PT_LOAD FLAGS((1 << 1) | (1 << 2));
}

SECTIONS
{
    . = 0xFFFFFFFF80000000;

    .limine_requests : {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } : limine_requests

    . = ALIGN(4K);

    .text : {
        *(.text.boot)
        *(.text .text.*)
    } : text

    . = ALIGN(4K);

    .rodata : {
        *(.rodata .rodata.*)
        *(.srodata .srodata.*)
    } : rodata

    . = ALIGN(4K);

    .data : {
        *(.data .data.*)
        *(.sdata .sdata.*)
    } : data

    . = ALIGN(4K);

    .bss : {
        __bss_start = .;
        *(.bss .bss.*)
        *(.sbss .sbss.*)
        *(COMMON)
        __bss_end = .;
    } : data

    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
    }
}