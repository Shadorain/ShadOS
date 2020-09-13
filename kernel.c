#include "kernel.h"

// Need function that will ret value of each 16bit pixel of character that will be displayed on screen with color
// To do: must ret 16bit val, since TERMINAL_BUFFER is of 16bit
static UINT16 VGA_DefaultEntry(unsigned char ch_to_print) {
    // Left shift will convert white_color val to a drawable char val in pixel format encoding with that char that we want to print
    // As to why bitwise took OR instead of AND
    return (UINT16)ch_to_print | (UINT16)WHITE_COLOR << 8;
}

// Main function which is called in boot.S
void KERNEL_MAIN() {
    // Init TERMINAL_BUFFER ptr, points to VGA addr
    // Need to cast to UINT16 type
    TERMINAL_BUFFER = (UINT16*)VGA_ADDRESS;

    // Now can access a VGA using simple array accessing using an index
    // Here need a val to be able to print in pixel format
    TERMINAL_BUFFER[0] = VGA_DefaultEntry('H');
    TERMINAL_BUFFER[1] = VGA_DefaultEntry('e');
    TERMINAL_BUFFER[2] = VGA_DefaultEntry('l');
    TERMINAL_BUFFER[3] = VGA_DefaultEntry('l');
    TERMINAL_BUFFER[4] = VGA_DefaultEntry('o');
    TERMINAL_BUFFER[5] = VGA_DefaultEntry(' ');
    TERMINAL_BUFFER[6] = VGA_DefaultEntry('S');
    TERMINAL_BUFFER[7] = VGA_DefaultEntry('h');
    TERMINAL_BUFFER[8] = VGA_DefaultEntry('a');
    TERMINAL_BUFFER[9] = VGA_DefaultEntry('d');
    TERMINAL_BUFFER[10] = VGA_DefaultEntry('o');
    TERMINAL_BUFFER[11] = VGA_DefaultEntry('k');
    TERMINAL_BUFFER[12] = VGA_DefaultEntry('n');
    TERMINAL_BUFFER[13] = VGA_DefaultEntry('i');
    TERMINAL_BUFFER[14] = VGA_DefaultEntry('g');
    TERMINAL_BUFFER[15] = VGA_DefaultEntry('h');
    TERMINAL_BUFFER[16] = VGA_DefaultEntry('t');
    TERMINAL_BUFFER[17] = VGA_DefaultEntry('!');
}
