/* link.ld */
OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
 {
   . = 0x100000;
   .text : { *(.text) } /* All object .text sections merged into one from all files, same below
                         * Location counter: 0x100000 + size of .text sect */
   .data : { *(.data) }
   .bss  : { *(.bss)  }
 }
