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

void printf_color(char *str, UINT8 fg, UINT8 bg) {
    UINT32 i = 0;
    UINT8 fc, bc;
    fc = g_fg;
    bc = g_bg;
    g_fg = fg;
    g_bg = bg;
    while (str[i]) {
        printc(str[i]);
        i++;
    }
    g_fg = fc;
    g_bg = bc;
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

// -- GUI -- //
UINT16 GET_BOX_DRAWC (UINT8 chn, UINT8 fg, UINT8 bg) {
    UINT16 ax = 0;
    UINT8 ah = 0;

    ah = bg;
    ah <<= 4;
    ah |= fg;
    ax = ah;
    ax <<= 8;
    ax |= chn;

    return ax;
}

void GOTOXY (UINT16 x, UINT16 y) {
    VGA_INDEX = 80*y;
    VGA_INDEX += x;
}

void DRAW_GENERIC_BOX (UINT16 x, UINT16 y, UINT16 w, UINT16 h, UINT8 fg, UINT8 bg,
               UINT8 topLeft_ch, UINT8 topBottom_ch, UINT8 topRight_ch, UINT8 leftRightEdge_ch, 
               UINT8 bottomLeft_ch, UINT8 bottomRight_ch) {
    UINT32 i;

    // Increase VGA_INDEX to x and y loc
    VGA_INDEX = 90*y;
    VGA_INDEX += x;

    // Top-left box char
    VGA_BUFFER[VGA_INDEX] = GET_BOX_DRAWC(topLeft_ch, fg, bg);
    VGA_INDEX++;
    // top chars
    for (i = 0; i < w; i++) {
        VGA_BUFFER[VGA_INDEX] = GET_BOX_DRAWC(topBottom_ch, fg, bg);
        VGA_INDEX++;
    }

    // Top-right box char
    VGA_BUFFER[VGA_INDEX] = GET_BOX_DRAWC(topRight_ch, fg, bg);

    // Increase y for drawing next line
    y++;
    // goto \n
    VGA_INDEX = 80*y;
    VGA_INDEX += x;

    // left & right sides
    for (i = 0; i < h; i++) {
        // left edge char
        VGA_BUFFER[VGA_INDEX] = GET_BOX_DRAWC(leftRightEdge_ch, fg, bg);
        VGA_INDEX++;
        // Increase VGA_INDEX to width
        VGA_INDEX += w;
        // Right side chara
        VGA_BUFFER[VGA_INDEX] = GET_BOX_DRAWC(leftRightEdge_ch, fg, bg);
        // Go to \n
        y++;
        VGA_INDEX = 80*y;
        VGA_INDEX += x;
    }
    // Bottom-left char
    VGA_BUFFER[VGA_INDEX] = GET_BOX_DRAWC(bottomLeft_ch, fg, bg);
    VGA_INDEX++;
    // Bottom chars
    for (i = 0; i < w; i++) {
        VGA_BUFFER[VGA_INDEX] = GET_BOX_DRAWC(topBottom_ch, fg, bg);
        VGA_INDEX++;
    }
    // Bottom-right char
    VGA_BUFFER[VGA_INDEX] = GET_BOX_DRAWC(bottomRight_ch, fg, bg);
    VGA_INDEX = 0;
}

void DRAW_BOX (UINT8 boxType, UINT16 x, UINT16 y, UINT16 w, UINT16 h, UINT8 fg, UINT8 bg) {
    switch (boxType) {
        case BOX_SINGLEBORDER :
            DRAW_GENERIC_BOX(x, y, w, h, fg, bg, 218, 196, 191, 179, 192, 217);
            break;
        case BOX_DOUBLEBORDER :
            DRAW_GENERIC_BOX(x, y, w, h, fg, bg, 201, 205, 187, 186, 200, 188);
            break;
    }
}

void FILL_BOX (UINT8 ch, UINT16 x, UINT16 y, UINT16 w, UINT16 h, UINT8 col) {
    UINT32 i,j;

    for(i = 0; i < h; i++) {
        // Increase VGA_INDEX to x & y loc
        VGA_INDEX = 80*y;
        VGA_INDEX += x;

        for (j = 0; j < w; j++) {
            VGA_BUFFER[VGA_INDEX] = GET_BOX_DRAWC(ch, 0, col);
            VGA_INDEX++;
        }
        y++;
    }
}


// -- ASCII -- //
void printc_ascii (UINT8 fg, UINT8 bg) {
    UINT32 i;
    UINT16 ax = 0;
    UINT16 ah = 0;

    for (i = 1; i < 254; i++) {
        ax = 0;
        ah = bg;
        ah <<= 4;
        ah |= fg;
        ax = ah;
        ax <<= 8;
        ax |= i;

        printi(i);
        printc(' ');
        printc(ax);
        printc(' ');
    }
}


// -- Main -- //
// Main function which is called in boot.S
void KERNEL_MAIN() {
    // Init VGA with fg & bg
    INIT_VGA(MAGENTA, BLACK);

    // - DOS Box - {{{
    FILL_BOX(0, 0, 0, BOX_MAX_WIDTH - 8, 14, MAGENTA);
    DRAW_BOX(BOX_DOUBLEBORDER, 0, 0, BOX_MAX_WIDTH - 10, 12, WHITE, MAGENTA);

    GOTOXY(2, 1);
    printf_color("Welcome to SHADOS-x86 v1.0.3", BRIGHT_GREEN, MAGENTA);
    GOTOXY(2, 3);
    printf_color("Short introduction for new knights: ", WHITE, MAGENTA);
    GOTOXY(41, 3);
    printf_color("INTRO", BLUE, MAGENTA);
    GOTOXY(2, 4);
    printf_color("For supported shell commands: ", WHITE, MAGENTA);
    GOTOXY(32, 4);
    printf_color("HELP", YELLOW, MAGENTA);
    GOTOXY(2, 11);
    printf_color("The ShadoHub", BRIGHT_GREEN, MAGENTA);
    GOTOXY(20, 11);
    printf_color("https://Shadorian.github.io", YELLOW, MAGENTA);
    GOTOXY(0, 14);
    printf_color("%> ", WHITE, BLACK);
    // - }}}
    // - Text test - {{{
    /* printf("Hello Shadoknight!"); */
    /* print_newline(); */
    /* printf("Hit any key ..."); */
    /* print_newline(); */
    test_input();
    // - }}}
    // - GUI Show off - // {{{
    /* const char *str = "Boxes"; */

    /* GOTOXY((VGA_MAX_WIDTH/2)-strlen(str), 1); */
    /* printf_color("Shadobox", BRIGHT_BLUE, BLACK); */

    /* DRAW_BOX(BOX_DOUBLEBORDER, 0, 0, BOX_MAX_WIDTH, BOX_MAX_HEIGHT, BRIGHT_GREEN, BLACK); */

    /* DRAW_BOX(BOX_SINGLEBORDER, 5, 3, 20, 5, GREEN, BLACK); */
    /* GOTOXY(10,6); */
    /* printf_color("Hello Shado", BRIGHT_CYAN, BLACK); */

    /* FILL_BOX(NULL, 36, 5, 30, 10, RED); */

    /* FILL_BOX(1, 6, 16, 30, 4, BRIGHT_RED); */
    /* DRAW_BOX(BOX_DOUBLEBORDER, 6, 16, 28, 3, BLUE, GREEN); */
    // - }}}
    /* printc_ascii(BRIGHT_MAGENTA, BLACK); */
}
