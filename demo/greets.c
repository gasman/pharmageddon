#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "video.h"
#include "stb_image.h"
#include "greets.h"
#include "gfx.h"

video greetsvid;

static unsigned char *font;

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
static unsigned char *text;
static int font_height, font_width;

static char *message = "                                           These folks deserve their names in lights... DUB, Slipstream, Aardbei, T$, Jumalauta, Oxygene, ThePetsMode, Truck, RC55, AYCE, Onslaught, Rift, TiTAN, Loonies, Pandacube, Bitshifters, CSC, 5711, Farbrausch, TUHB, Furry Trash Group, Spaceballs, SMFX, F#READY, Desire, Trepaan, Poo-Brain, RBBS, D.Fox, Dojoe, Conspiracy, ScenePT All Stars, Logicoma... and you! Please take your prescription.                                                                      ";

void greets_init(void) {
    video_open("../assets/pharma5.mpg", &greetsvid);
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

void greets_frame(uint32_t *pixels, uint32_t time) {
    video_frame(&greetsvid, pixels, time);

    unsigned char *text_ptr = text + font_height * (time/15);
    unsigned char *text_bar_start = text + font_height * 350;

    for (int x = 0; x < 192; x++) {
        int y_top = (
            (96 - font_height / 2)
            + (int)(8 * sin(
                ((double)x / 20) - ((double)time / 200)
            ))
        );
        uint32_t *screen_ptr = pixels + 192 * y_top + x;
        for (int y = 0; y < font_height; y++) {
            if (text_ptr >= text_bar_start) {
                *screen_ptr = 0x00010000 * (*text_ptr);
            }
            text_ptr++;
            screen_ptr += 192;
        }
    }
}
