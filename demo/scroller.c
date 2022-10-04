#include <stdio.h>
#include <stdlib.h>
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

static int font_height, font_width;

void scroller_init(void) {
    int n;
    /* load as greyscale bitmap */
    font = stbi_load("../assets/chicago.png", &font_width, &font_height, &n, 1);

    unsigned int offset = 0;
    for (int i=0; i < FONT_LENGTH; i++) {
        char_offsets[i] = offset;
        offset += (unsigned int)char_widths[i];
    }
}

static char *message = "Hello world!";

void scroller_frame(uint32_t *pixels, uint32_t time) {
    char *message_ptr = message;
    uint32_t *screen_ptr = pixels + 192 * (96 - font_height / 2);
    while (*message_ptr != 0) {
        char message_char = *message_ptr;
        unsigned int char_offset = char_offsets[message_char - 32];
        unsigned char char_width = char_widths[message_char - 32];

        uint32_t *screen_row_ptr = screen_ptr;
        unsigned char *font_char_ptr = font + char_offset;
        for (int y = 0; y < font_height; y++) {
            for (unsigned char x = 0; x < char_width; x++) {
                screen_row_ptr[x] = 0x00010000 * (uint32_t)(font_char_ptr[x]);
            }
            screen_row_ptr += 192;
            font_char_ptr += font_width;
        }
        screen_ptr += char_width;
        message_ptr++;
    }
}
