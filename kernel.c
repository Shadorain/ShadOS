#include "kernel.h"
#include "utils.h"
#include "chars.h"
#include "kbd.h"
#include "ttt.h"

uint32 vga_index; // video buffer array index
static uint32 nl_index = 1; // Newline counter
uint8 g_fg = WHITE, g_bg = BLUE; // fg & bg default values
int digit_ascii_codes[10] = {0x30, 0x31, 0x32, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39}; // ASCII digit codes for integers

// -- General Functions -- {{{
/* 16bit video buffer elements (AX)
   8bit(AH) higher:
     lower 4bits -- fg color
     higher 4bits -- bg color
   8bit (AL) lower:
     ASCII character to print */
uint16 vga_entry(unsigned char ch, uint8 fg, uint8 bg) {
    uint16 ax = 0;
    uint8 ah = 0, al = 0;

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
void clr_vga_buffer(uint16 **buffer, uint8 fg, uint8 bg) {
    for(uint32 i=0; i < BUFSIZE; i++)
        (*buffer)[i] = vga_entry(NULL, fg, bg);
    nl_index = 1;
    VGA_INDEX = 0;
}

void clr_display(uint8 fg, uint8 bg) {
    clr_vga_buffer(&VGA_BUFFER, fg, bg);
}

// Init VGA_BUFFER
void init_vga(uint8 fg, uint8 bg) {
    // points to VGA_ADDRESS
    VGA_BUFFER = (uint16*)VGA_ADDRESS;
    // Clr buffer
    clr_vga_buffer(&VGA_BUFFER, fg, bg);
    g_fg = fg;
    g_bg = bg;
}

// Assign ASCII char to video buffer
void printc(char ch) {
    VGA_BUFFER[VGA_INDEX] = vga_entry(ch, g_fg, g_bg);
    VGA_INDEX++;
}

void printf_color(char *str, uint8 fg, uint8 bg) {
    uint32 i = 0;
    uint8 fc, bc;
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
    uint32 i = 0;
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
        clr_vga_buffer(&VGA_BUFFER, g_fg, g_bg);
    }
    VGA_INDEX = 80*nl_index;
    nl_index++;
}

uint8 inb(uint16 port) {
    uint8 ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
    return ret;
}

void outb(uint16 port, uint8 data) {
    asm volatile("outb %0, %1" : "=a"(data) : "d"(port));
}

void gotoxy (uint16 x, uint16 y) {
    VGA_INDEX = 80*y;
    VGA_INDEX += x;
}

// -- }}}
// -- Input -- {{{
char get_input_keycode() {
    char ch = 0;
    while ((ch = inb(KBD_PORT)) != 0)
        if(ch > 0)
            return ch;
    return ch;
}

// Keeps CPU busy so that io port wont be proced by CPU, using looping counter
void wait_for_io(uint32 timer_count) {
    while(1) {
        asm volatile("nop");
        timer_count--;
        if (timer_count <= 0) 
            break;
    }
}

// Sleep timer
void sleep(uint32 timer_count) {
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
// -- }}}
// -- ASCII -- {{{
void printc_ascii (uint8 fg, uint8 bg) {
    uint32 i;
    uint16 ax = 0;
    uint16 ah = 0;

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
// -- }}}
// -- Main -- {{{
// Main function which is called in boot.S
void kernel_main() {
    // Init VGA with fg & bg
    init_vga(WHITE, BLACK);

    // - TTT - {{{
    byte ans = KEY_Y;
    while (ans == KEY_Y) {
        clr_display(WHITE, BLACK);
        launch_game();

        sleep(0x02FFFFFF);
        clr_display(WHITE, BLACK);
        printf("Would you like to play again? (y/n) : ");
        ans = get_input_keycode();
        sleep(0x04FFFFFF);
        if (ans == KEY_Y)
            continue;
        else if (ans == KEY_N) {
            print_newline();
            printf("Thanks for playing!");
            break;
        } else {
            print_newline();
            printf("Invalid Choice!");
            print_newline();
            printf("Exited...");
            break;
        }
    }
    // - }}}
    // - DOS Box - {{{
    /* fill_box(0, 0, 0, BOX_MAX_WIDTH - 8, 14, MAGENTA); */
    /* draw_box(BOX_DOUBLEBORDER, 0, 0, BOX_MAX_WIDTH - 10, 12, WHITE, MAGENTA); */

    /* gotoxy(2, 1); */
    /* printf_color("Welcome to SHADOS-x86 v1.0.3", BRIGHT_GREEN, MAGENTA); */
    /* gotoxy(2, 3); */
    /* printf_color("Short introduction for new knights: ", WHITE, MAGENTA); */
    /* gotoxy(41, 3); */
    /* printf_color("INTRO", BLUE, MAGENTA); */
    /* gotoxy(2, 4); */
    /* printf_color("For supported shell commands: ", WHITE, MAGENTA); */
    /* gotoxy(32, 4); */
    /* printf_color("HELP", YELLOW, MAGENTA); */
    /* gotoxy(2, 11); */
    /* printf_color("The ShadoHub", BRIGHT_GREEN, MAGENTA); */
    /* gotoxy(20, 11); */
    /* printf_color("https://Shadorian.github.io", YELLOW, MAGENTA); */
    /* gotoxy(0, 14); */
    /* printf_color("%> ", WHITE, BLACK); */
    // - }}}
    // - Text test - {{{
    /* printf("Hello Shadoknight!"); */
    /* print_newline(); */
    /* printf("Hit any key ..."); */
    /* print_newline(); */
    /* test_input(); */
    // - }}}
    // - GUI Show off - // {{{
    /* const char *str = "Boxes"; */

    /* gotoxy((VGA_MAX_WIDTH/2)-strlen(str), 1); */
    /* printf_color("Shadobox", BRIGHT_BLUE, BLACK); */

    /* draw_box(BOX_DOUBLEBORDER, 0, 0, BOX_MAX_WIDTH, BOX_MAX_HEIGHT, BRIGHT_GREEN, BLACK); */

    /* draw_box(BOX_SINGLEBORDER, 5, 3, 20, 5, GREEN, BLACK); */
    /* gotoxy(10,6); */
    /* printf_color("Hello Shado", BRIGHT_CYAN, BLACK); */

    /* fill_box(NULL, 36, 5, 30, 10, RED); */

    /* fill_box(1, 6, 16, 30, 4, BRIGHT_RED); */
    /* draw_box(BOX_DOUBLEBORDER, 6, 16, 28, 3, BLUE, GREEN); */
    // - }}}
    /* printc_ascii(BRIGHT_MAGENTA, BLACK); */
}
// -- }}}
