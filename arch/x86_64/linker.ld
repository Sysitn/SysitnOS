OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
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
    }

    . += CONSTANT(MAXPAGESIZE);

    .text : {
        *(.text .text.*)
    } : text

    . += CONSTANT(MAXPAGESIZE);

    .rodata : {
        *(.rodata .rodata.*)
    } : rodata

    . += CONSTANT(MAXPAGESIZE);

    .data : {
        *(.data .data.*)
    } : data

    . += CONSTANT(MAXPAGESIZE);

    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    } : data

    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
    }
}