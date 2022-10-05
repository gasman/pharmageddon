#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gfx.h"
#include "scroller.h"
#include "stb_image.h"

unsigned char *font;

#define FONT_LENGTH 91

static unsigned char char_widths[FONT_LENGTH] = {
    4, 4, 5, 9, 7, 11, 9, 4, 5, 5, 7, 7, 5, 7, 4, 7,
    8, 5, 8, 8, 9, 8, 7, 8, 8, 8,
    4, 4, 7, 5, 7, 8, 11,
    8, 8, 8, 8, 7, 6, 8, 8, 4, 8, 9, 7, 12,
    9, 8, 8, 8, 8, 6, 7, 8, 8, 12, 8, 8, 8,
    5, 7, 5, 6, 7, 5,
    8, 8, 7, 8, 7, 6, 8, 8, 3, 7, 8, 4, 12,
    8, 8, 8, 8, 6, 7, 6, 8, 8, 12, 8, 8, 8
};
static unsigned int char_offsets[FONT_LENGTH];
unsigned char *text;

static int font_height, font_width;

static char *message = "Hello party people! We've all seen those videos of extremely demoish pharmacy signs, so it seemed only fair to make a demo about it...";

void scroller_init(void) {
    int n;
    /* load as greyscale bitmap */
    font = stbi_load("../assets/chicago.png", &font_width, &font_height, &n, 1);

    unsigned int offset = 0;
    for (int i=0; i < FONT_LENGTH; i++) {
        char_offsets[i] = offset;
        offset += (unsigned int)char_widths[i];
    }

    /* find total text length in pixels */
    unsigned int text_width = 192;
    for (char *message_ptr = message; *message_ptr != 0; message_ptr++) {
        text_width += (unsigned int)char_widths[(*message_ptr) - 32];
    }

    text = (unsigned char *)malloc(text_width * font_height * sizeof(unsigned char));
    memset(text, 0, 192 * font_height);

    /* render text to image */
    unsigned char *text_ptr = text + 192 * font_height;
    for (char *message_ptr = message; *message_ptr != 0; message_ptr++) {
        unsigned int char_offset = char_offsets[(*message_ptr) - 32];
        unsigned char char_width = char_widths[(*message_ptr) - 32];
        unsigned char *font_char_ptr = font + char_offset;

        for (unsigned char y = 0; y < char_width; y++) {
            unsigned char *font_row_ptr = font_char_ptr;
            for (int x = 0; x < font_height; x++) {
                *text_ptr = *font_row_ptr;
                text_ptr++;
                font_row_ptr += font_width;
            }
            font_char_ptr++;
        }
    }
}

void scroller_frame(uint32_t *pixels, uint32_t time) {
    unsigned char *text_ptr = text + font_height * (time/20);

    for (int x = 0; x < 192; x++) {
        uint32_t *screen_ptr = pixels + 192 * (96 - font_height / 2) + x;
        for (int y = 0; y < font_height; y++) {
            *screen_ptr = 0x00010000 * (*text_ptr);
            text_ptr++;
            screen_ptr += 192;
        }
    }
}
