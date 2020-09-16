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
uint16 *VGA_BUFFER;
extern uint32 VGA_INDEX;

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

extern void clr_display(uint8, uint8);
extern void print_newline();
extern void printc(char);
extern void printf(char *);
extern void printf_color(char *, uint8, uint8);
extern void printi(int);
extern uint8 inb(uint16);
extern void outb(uint16, uint8);
extern byte get_input_keycode();
extern void sleep(uint32);
extern void gotoxy(uint16, uint16);

#endif
