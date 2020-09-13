#ifndef KERNEL_H
#define KERNEL_H

// Define 8,16,32 bit unsigned type
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;

// Define a VGA address
#define VGA_ADDRESS 0xB8000
// Define buffer size
#define BUFSIZE 2200
// Define null value
#define NULL 0

enum VGA_COLOR {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,
    DARK_GREY,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    YELLOW,
    WHITE,
};

// Need buffer that points to a VGA, can also be called as VGA_BUFFER 
UINT16 *TERMINAL_BUFFER;

#endif
