OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

PHDRS
{
    text    PT_LOAD    FLAGS((1 << 0) | (1 << 2)) ; /* Execute + Read */
    rodata  PT_LOAD    FLAGS((1 << 2)) ;            /* Read only */
    data    PT_LOAD    FLAGS((1 << 1) | (1 << 2)) ; /* Write + Read */
}

SECTIONS
{
    . = 0xFFFFFFFF80000000;

    __ktext_base = .;

    .text : {
        *(.text .text.*)
    } :text

    __ktext_end = .;

    . += CONSTANT(MAXPAGESIZE);

    __krodata_base = .;

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    __krodata_end = .;

    . += CONSTANT(MAXPAGESIZE);

    __kdata_base = .;

    .data : {
        *(.data .data.*)
    } :data

    .bss : {
        *(COMMON)
        *(.bss .bss.*)
    } :data

    __kdata_end = .;

    /DISCARD/ : {
        *(.note .note.*)
    }
}
