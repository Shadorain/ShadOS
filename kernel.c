#include "kernel.h"
#include "kbd.h"

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
static UINT16 VGA_ENTRY(unsigned char ch, UINT8 fg, UINT8 bg) {
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

// Increase VGA_INDEX by width of row (80)
void print_newline() {
    if(nl_index >= 55) {
        nl_index = 0;
        CLR_VGA_BUFFER(&VGA_BUFFER, g_fg, g_bg);
    }
    VGA_INDEX = 80*nl_index;
    nl_index++;
}

// Assign ASCII char to video buffer
void printc(char ch) {
    VGA_BUFFER[VGA_INDEX] = VGA_ENTRY(ch, g_fg, g_bg);
    VGA_INDEX++;
}

UINT32 strlen(const char* str) {
    UINT32 length = 0;
    while (str[length])
        length++;
    return length;
}

UINT32 digit_count(int num) {
    UINT32 count = 0;
    if(num==0)
        return 1;
    while(num > 0) {
        count++;
        num = num/10;
    }
    return count;
}

void itoa(int num, char *number) {
    int dgcount = digit_count(num);
    int i = dgcount - 1;
    char x;
    if(num==0 && dgcount==1) {
        number[0] = '0';
        number[1] = '\0';
    } else { 
        while(num != 0) {
            x = num % 10;
            number[i] = x + '0';
            i--;
            num = num / 10;
        }
        number[dgcount] = '\0';
    }
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

// Main function which is called in boot.S
void KERNEL_MAIN() {
    // Init VGA with fg & bg
    INIT_VGA(MAGENTA, BLACK);

    // Can print strings, chars, and ints with new functions
    printf("Hello Shadoknight!");
    print_newline();
    printi(1337);
    print_newline();
    printf("Cya!");
}
