#include <math.h>
#include <stdlib.h>
#include "gfx.h"
#include "amigaball.h"

#define BACKGROUND_GREY 0x88888800
#define SHADOW_GREY 0x66666600

gfx_image amigaball[5];

double y_positions[10000];
double x_positions[10000];

void amigaball_init(void) {
    gfx_loadimage("../assets/amiga1.png", &amigaball[0]);
    gfx_loadimage("../assets/amiga2.png", &amigaball[1]);
    gfx_loadimage("../assets/amiga3.png", &amigaball[2]);
    gfx_loadimage("../assets/amiga4.png", &amigaball[3]);
    gfx_loadimage("../assets/amiga5.png", &amigaball[4]);

    double y = 92;
    double x = 28;
    double yv = 0;
    double xv = 0.0333;
    for (int t = 0; t < 10000; t++) {
        y += yv;
        x += xv;
        yv += 0.0002;
        y_positions[t] = y;
        x_positions[t] = x;
        if (y > 100 && (t < 1500 | t > 3700)) {
            yv = -yv;
        } else if (y > 164) {
            yv = -yv;
        }
        if (t == 2250 || t == 2700) xv = -xv;
    }
}

void draw_shadow(uint32_t *pixels, gfx_image *image, int x, int y) {
    int screen_y = y;
    unsigned char *image_data_row = image->data;

    for (int image_y = 0; image_y < image->height; image_y++) {
        if (screen_y >= 192) return;
        if (screen_y >= 0) {
            uint32_t *screen_pos = pixels + screen_y * 192 + x;
            int screen_x = x;
            unsigned char *image_data_pos = image_data_row;
            for (int image_x = 0; image_x < image->width; image_x++) {
                if (screen_x >= 192) break;
                if (screen_x >= 0 && *screen_pos == BACKGROUND_GREY && image_data_pos[0] != 0) {
                    *screen_pos = SHADOW_GREY;
                }
                screen_pos++;
                image_data_pos += 4;
                screen_x++;
            }
        }
        screen_y++;
        image_data_row += image->width * 4;
    }
}

void amigaball_frame(uint32_t *pixels, uint32_t time) {
    gfx_cls(pixels, BACKGROUND_GREY);
    for (int x = 0; x < 192; x += 16) {
        gfx_line(pixels, x, 0, x, 191, 0xa1279f00);
    }
    for (int y = 0; y < 192; y += 16) {
        gfx_line(pixels, 0, y, 191, y, 0xa1279f00);
    }

    int ball_x = x_positions[time % 10000];
    int ball_y = y_positions[time % 10000];
    gfx_image image = amigaball[(time / 50) % 5];

    draw_shadow(pixels, &image, ball_x - 16, ball_y - 28);
    gfx_drawimage(pixels, &image, ball_x - 32, ball_y - 32);
}
