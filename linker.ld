/* First, set entry point of bootloader main function */
ENTRY(_start)

SECTIONS
{
    /* Need atleast 1MB size */
    . = 1M;

    /* Size of text section needs atleast 4Kk size for each section */
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    /* Read only data */
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    /* Data section */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    /* BSS section */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}
