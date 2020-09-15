#ifndef KERNEL_H
#define KERNEL_H

#include "types.h"

// Define a VGA address
#define VGA_ADDRESS 0xB8000
// Define buffer size
#define BUFSIZE 2200
// Define null value
#define NULL 0

// Buffer that points to a VGA
UINT16 *VGA_BUFFER;

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

#include "kbd.h"

#endif
