#include "kernel.h"

/* 16bit video buffer elements (AX)
   8bit(AH) higher:
     lower 4bits -- fg color
     higher 4bits -- bg color
   8bit (AL) lower:
     ASCII character to print */
static UINT16 VGA_DefaultEntry(unsigned char ch, UINT8 fg, UINT8 bg) {
    UINT16 ax = 0;
    UINT8 ah = 0, al = 0;

    ah = bg;
    ah <<= 4;
    ah |= fg;
    ax = ah;
    ax <<= 8;
    al = ch;
    ax |= al;

    return ax;
}

// Clear vid buffer array function
void CLR_VGA_BUFFER(UINT16 **buffer, UINT8 fg, UINT8 bg) {
    for(UINT32 i=0; i < BUFSIZE; i++)
        (*buffer)[i] = VGA_DefaultEntry(NULL, fg, bg);
}

// Init VGA_BUFFER
void INIT_VGA(UINT8 fg, UINT8 bg) {
    // points to VGA_ADDRESS
    VGA_BUFFER = (UINT16*)VGA_ADDRESS;
    // Clr buffer
    CLR_VGA_BUFFER(&VGA_BUFFER, fg, bg);
}

// Main function which is called in boot.S
void KERNEL_MAIN() {
    // Init VGA with fg & bg
    INIT_VGA(WHITE, BLACK);

    // Now can access a VGA using simple array accessing using an index
    // Here need a val to be able to print in pixel format
    VGA_BUFFER[0] = VGA_DefaultEntry('H', WHITE, BLACK);
    VGA_BUFFER[1] = VGA_DefaultEntry('e', WHITE, BLACK);
    VGA_BUFFER[2] = VGA_DefaultEntry('l', WHITE, BLACK);
    VGA_BUFFER[3] = VGA_DefaultEntry('l', WHITE, BLACK);
    VGA_BUFFER[4] = VGA_DefaultEntry('o', WHITE, BLACK);
    VGA_BUFFER[5] = VGA_DefaultEntry(' ', WHITE, BLACK);
    VGA_BUFFER[6] = VGA_DefaultEntry('S', MAGENTA, BLACK);
    VGA_BUFFER[7] = VGA_DefaultEntry('h', MAGENTA, BLACK);
    VGA_BUFFER[8] = VGA_DefaultEntry('a', MAGENTA, BLACK);
    VGA_BUFFER[9] = VGA_DefaultEntry('d', MAGENTA, BLACK);
    VGA_BUFFER[10] = VGA_DefaultEntry('o', MAGENTA, BLACK);
    VGA_BUFFER[11] = VGA_DefaultEntry('k', MAGENTA, BLACK);
    VGA_BUFFER[12] = VGA_DefaultEntry('n', MAGENTA, BLACK);
    VGA_BUFFER[13] = VGA_DefaultEntry('i', MAGENTA, BLACK);
    VGA_BUFFER[14] = VGA_DefaultEntry('g', MAGENTA, BLACK);
    VGA_BUFFER[15] = VGA_DefaultEntry('h', MAGENTA, BLACK);
    VGA_BUFFER[16] = VGA_DefaultEntry('t', MAGENTA, BLACK);
    VGA_BUFFER[17] = VGA_DefaultEntry('!', MAGENTA, BLACK);
}
