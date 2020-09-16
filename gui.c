#include "kernel.h"
#include "types.h"
#include "utils.h"
#include "gui.h"

// -- GUI -- //
uint16 get_box_drawc (uint8 chn, uint8 fg, uint8 bg) {
    uint16 ax = 0;
    uint8 ah = 0;

    ah = bg;
    ah <<= 4;
    ah |= fg;
    ax = ah;
    ax <<= 8;
    ax |= chn;

    return ax;
}

void draw_gen_box (uint16 x, uint16 y, uint16 w, uint16 h, uint8 fg, uint8 bg,
               uint8 topLeft_ch, uint8 topBottom_ch, uint8 topRight_ch, uint8 leftRightEdge_ch, 
               uint8 bottomLeft_ch, uint8 bottomRight_ch) {
    uint32 i;

    // Increase VGA_INDEX to x and y loc
    VGA_INDEX = 90*y;
    VGA_INDEX += x;

    // Top-left box char
    VGA_BUFFER[VGA_INDEX] = get_box_drawc(topLeft_ch, fg, bg);
    VGA_INDEX++;
    // top chars
    for (i = 0; i < w; i++) {
        VGA_BUFFER[VGA_INDEX] = get_box_drawc(topBottom_ch, fg, bg);
        VGA_INDEX++;
    }

    // Top-right box char
    VGA_BUFFER[VGA_INDEX] = get_box_drawc(topRight_ch, fg, bg);

    // Increase y for drawing next line
    y++;
    // goto \n
    VGA_INDEX = 80*y;
    VGA_INDEX += x;

    // left & right sides
    for (i = 0; i < h; i++) {
        // left edge char
        VGA_BUFFER[VGA_INDEX] = get_box_drawc(leftRightEdge_ch, fg, bg);
        VGA_INDEX++;
        // Increase VGA_INDEX to width
        VGA_INDEX += w;
        // Right side chara
        VGA_BUFFER[VGA_INDEX] = get_box_drawc(leftRightEdge_ch, fg, bg);
        // Go to \n
        y++;
        VGA_INDEX = 80*y;
        VGA_INDEX += x;
    }
    // Bottom-left char
    VGA_BUFFER[VGA_INDEX] = get_box_drawc(bottomLeft_ch, fg, bg);
    VGA_INDEX++;
    // Bottom chars
    for (i = 0; i < w; i++) {
        VGA_BUFFER[VGA_INDEX] = get_box_drawc(topBottom_ch, fg, bg);
        VGA_INDEX++;
    }
    // Bottom-right char
    VGA_BUFFER[VGA_INDEX] = get_box_drawc(bottomRight_ch, fg, bg);
    VGA_INDEX = 0;
}

void draw_box (uint8 boxType, uint16 x, uint16 y, uint16 w, uint16 h, uint8 fg, uint8 bg) {
    switch (boxType) {
        case BOX_SINGLEBORDER :
            draw_gen_box(x, y, w, h, fg, bg, 218, 196, 191, 179, 192, 217);
            break;
        case BOX_DOUBLEBORDER :
            draw_gen_box(x, y, w, h, fg, bg, 201, 205, 187, 186, 200, 188);
            break;
    }
}

void fill_box (uint8 ch, uint16 x, uint16 y, uint16 w, uint16 h, uint8 col) {
    uint32 i,j;

    for(i = 0; i < h; i++) {
        // Increase VGA_INDEX to x & y loc
        VGA_INDEX = 80*y;
        VGA_INDEX += x;

        for (j = 0; j < w; j++) {
            VGA_BUFFER[VGA_INDEX] = get_box_drawc(ch, 0, col);
            VGA_INDEX++;
        }
        y++;
    }
}
