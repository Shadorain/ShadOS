#include "kernel.h"
#include "utils.h"
#include "chars.h"

// Video buffer array index
UINT32 VGA_INDEX;
// Newline counter
static UINT32 nl_index = 1;
// fg & bg default values
UINT8 g_fg = WHITE, g_bg = BLUE;
// ASCII digit codes for integers
int digit_ascii_codes[10] = {0x30, 0x31, 0x32, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

/* 16bit video buffer elements (AX)
   8bit(AH) higher:
     lower 4bits -- fg color
     higher 4bits -- bg color
   8bit (AL) lower:
     ASCII character to print */
UINT16 VGA_ENTRY(unsigned char ch, UINT8 fg, UINT8 bg) {
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
        (*buffer)[i] = VGA_ENTRY(NULL, fg, bg);
    nl_index = 1;
    VGA_INDEX = 0;
}

// Init VGA_BUFFER
void INIT_VGA(UINT8 fg, UINT8 bg) {
    // points to VGA_ADDRESS
    VGA_BUFFER = (UINT16*)VGA_ADDRESS;
    // Clr buffer
    CLR_VGA_BUFFER(&VGA_BUFFER, fg, bg);
    g_fg = fg;
    g_bg = bg;
}

// Assign ASCII char to video buffer
void printc(char ch) {
    VGA_BUFFER[VGA_INDEX] = VGA_ENTRY(ch, g_fg, g_bg);
    VGA_INDEX++;
}

// Print string by using printc
void printf(char *str) {
    UINT32 i = 0;
    while(str[i]) {
        printc(str[i]);
        i++;
    }
}

// Print int by converting to str
void printi(int num) {
    char str_num[digit_count(num)+1];
    itoa(num, str_num);
    printf(str_num);
}

// Increase VGA_INDEX by width of row (80)
void print_newline() {
    if(nl_index >= 55) {
        nl_index = 0;
        CLR_VGA_BUFFER(&VGA_BUFFER, g_fg, g_bg);
    }
    VGA_INDEX = 80*nl_index;
    nl_index++;
}

UINT8 inb(UINT16 port) {
    UINT8 ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
    return ret;
}

void outb(UINT16 port, UINT8 data) {
    asm volatile("outb %0, %1" : "=a"(data) : "d"(port));
}

char get_input_keycode() {
    char ch = 0;
    while ((ch = inb(KBD_PORT)) != 0)
        if(ch > 0)
            return ch;
    return ch;
}

// Keeps CPU busy so that io port wont be proced by CPU, using looping counter
void wait_for_io(UINT32 timer_count) {
    while(1) {
        asm volatile("nop");
        timer_count--;
        if (timer_count <= 0) 
            break;
    }
}

// Sleep timer
void sleep(UINT32 timer_count) {
    wait_for_io(timer_count);
}

void test_input() {
    char ch = 0;
    char keycode = 0;
    do {
        keycode = get_input_keycode();
        if(keycode == KEY_ENTER)
            print_newline();
        else {
            ch = get_ascii_char(keycode);
            printc(ch);
        }
        sleep(0x02FFFFFF);
    } while (ch > 0);
}

// Main function which is called in boot.S
void KERNEL_MAIN() {
    // Init VGA with fg & bg
    INIT_VGA(MAGENTA, BLACK);

    // Can print strings, chars, and ints with new functions
    printf("Hello Shadoknight!");
    print_newline();
    printf("Hit any key ...");
    print_newline();
    test_input();
}
