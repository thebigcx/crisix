OUTPUT_FORMAT(elf64-x86-64)

ENTRY(kmain_st2)

SECTIONS
{
    __highmem = 0xffffffff80000000;

    . = __highmem;
    . += 0x200000;

    _kernel_start = .;

    .stivale2hdr : AT(ADDR(.stivale2hdr) - __highmem)
    {
        KEEP(*(.stivale2hdr))
    }

    .text : AT(ADDR(.text) - __highmem)
    {
        *(.text*)
        . = ALIGN(0x1000);
    }

    . += 0x1000;

    .rodata : AT(ADDR(.rodata) - __highmem)
    {
        *(.rodata*)
        . = ALIGN(0x1000);
    }

    . += 0x1000;

    .data : AT(ADDR(.data) - __highmem)
    {
        *(.data*)
        . = ALIGN(0x1000);
    }

    .dynamic :
    {
        *(.dynamic)
        . = ALIGN(0x1000);
    }

    .bss : AT(ADDR(.bss) - __highmem)
    {
        *(COMMON)
        *(.bss*)
        . = ALIGN(0x1000);
    }

    _kernel_end = .;
}
