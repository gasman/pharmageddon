#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

static char *message = "                                           Greetings party people! Are you tired of your regular pharmacy signs? Do you wish that they could be a bit more demosceneish? Time to make a demo about it...                                               ";

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

#define CROSSCOUNT 8

// parametric function for the x position on a cross. ranges from 0-1. used for drawing the rotating dots.
double crossx(double t) {
    t = fmod(t, 1.0);
    if(t<(1/12.0)) {
        return t*4;
    } else if(t<(2/12.0)) {
        return (double)1/3.0;
    } else if (t<(3/12.0)) {
        return t*4-(1/3.0);
    } else if (t<(4/12.0)) {
        return 2/3.0;
    } else if (t<(5/12.0)) {
        return t*4-(2/3.0);
    } else if (t<(6/12.0)) {
        return 1;
    } else if (t<(7/12.0)) {
        return -(t*4)+3.0;
    } else if (t<(8/12.0)) {
        return 2/3.0;
    } else if (t<(9/12.0)) {
        return -(t*4)+3.0+(1/3.0);
    } else if (t<(10/12.0)) {
        return 1/3.0;
    } else if (t<(11/12.0)) {
        return -(t*4)+3.0+(2/3.0);
    } else {
        return 0;
    }
}

// parametric function for the y position on a cross. ranges from 0-1. used for drawing the rotating dots.
double crossy(double t) {
    return crossx(t+9/12.0);
}

// x and y are the point of the middle of the cross
void plotCross(uint32_t *pixels, float x, float y, float h, float bpos, float bsize, int color) {
    double t;
    for(double i=0;i<h*8;i++) {
        t = i/h/8;
        if(t > bpos && t < bpos+bsize) {
            // leave black
        } else {
            gfx_putpixel(pixels, (int)(crossx(t)*h+x-(h/2)), (int)(crossy(t)*h+y-(h/2)), color);
        }
    }

}

void scroller_frame(uint32_t *pixels, uint32_t time) {
    unsigned char *text_ptr = text + font_height * (time/20);
    unsigned char *text_bar_start = text + font_height * 350;

    double pos = fmod(((double)time)/5000.0, 1.0);

    gfx_cls(pixels, 0x00000000);

    for(int i=0;i<CROSSCOUNT;i++) {
        double scale = fmod((-i*(192/8)) + ((double)time)/10, 192.0);
        if(i%2 == 1) {
            plotCross(pixels,96,96,scale,pos,0.05,i*32<<16);
        } else {
            plotCross(pixels,96,96,scale,-pos+1,0.05,i*32<<16);
        }
    }

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
