#include <math.h>
#include <stdlib.h>
#include "gfx.h"
#include "credits.h"
#include "stb_image.h"

#define SPRITE_WIDTH 192
#define SPRITE_HEIGHT 64
#define NAME_GAP 2000

unsigned char *sprites[7];

void credits_init(void) {
    int w, h, n;
    /* load as greyscale bitmap */
    sprites[0] = stbi_load("../assets/gasman.png", &w, &h, &n, 1);
    sprites[1] = stbi_load("../assets/nihilazo.png", &w, &h, &n, 1);
    sprites[2] = stbi_load("../assets/superogue.png", &w, &h, &n, 1);
    sprites[3] = stbi_load("../assets/aldroid.png", &w, &h, &n, 1);
    sprites[4] = stbi_load("../assets/mantratronic.png", &w, &h, &n, 1);
    sprites[5] = stbi_load("../assets/raccoonviolet.png", &w, &h, &n, 1);
    sprites[6] = stbi_load("../assets/tobach.png", &w, &h, &n, 1);
}


void plot_sprite(uint32_t *pixels, unsigned char *image, double cx, double cy, double r, double s, unsigned char threshold, unsigned char intensity) {
    /* width / height as measured from centre */
    double w = SPRITE_WIDTH / 2.0;
    double h = SPRITE_HEIGHT / 2.0;

    /* calculate bounding box of rotated / scaled image */
    double cr = cos(r);
    double sr = sin(r);
    double wcr = fabs(s*w*cr);
    double hsr = fabs(s*h*sr);
    double hcr = fabs(s*h*cr);
    double wsr = fabs(s*w*sr);

    int min_x = (int)floor(cx - wcr - hsr);
    if (min_x < 0) min_x = 0;
    int max_x = (int)ceil(cx + wcr + hsr);
    if (max_x > 191) max_x = 191;
    int min_y = (int)floor(cy - hcr - wsr);
    if (min_y < 0) min_y = 0;
    int max_y = (int)ceil(cy + hcr + wsr);
    if (max_y > 191) max_y = 191;

    for (int sy = min_y; sy <= max_y; sy++) {
        for (int sx = min_x; sx <= max_x; sx++) {
            double rx = ((double)sx - cx) / s;
            double ry = ((double)sy - cy) / s;
            int ix = (int)round(rx * cr + ry * sr + w);
            int iy = (int)round(ry * cr - rx * sr + h);
            if (ix >= 0 && ix < SPRITE_WIDTH && iy >= 0 && iy < SPRITE_HEIGHT) {
                unsigned char level = image[iy * SPRITE_WIDTH + ix];
                if (level > threshold) {
                    uint32_t *scr_pos = pixels + sy * 192 + sx;
                    *scr_pos = intensity << 16;
                }
            }
        }
    }
}

void draw_name(uint32_t *pixels, unsigned char *image, uint32_t time) {
    double threshold;
    if (time > 1000) {
        threshold = 0.75;
    } else {
        threshold = 0.75 * ((double)time) / 1000;
    }

    double intensity;
    if (time < 2000) {
        intensity = 1.0;
    } else if (time < 4000) {
        intensity = 1.0 - ((double)(time - 2000) / 2000);
    } else {
        intensity = 0.0;
    }

    double z = 1.0 + (double)time / 1000.0;
    plot_sprite(pixels, image, 96.0, 96.0, (double)time / 1000.0, 1/z, (unsigned char)(128 * threshold), (unsigned char)(255 * intensity));

}

void credits_frame(uint32_t *pixels, uint32_t time) {
    int sprite_num = (time / NAME_GAP) % 7;
    uint32_t ntime = time % NAME_GAP;

    double flash;
    if (ntime > 400) {
        flash = 0;
    } else {
        flash = 1 - pow(((double)ntime) / 400, 0.5);
    }
    gfx_cls(pixels, (int)(flash*255) << 16);

    if (time > 2000) {
        draw_name(pixels, sprites[(sprite_num + 6) % 7], ntime + NAME_GAP);
    }
    draw_name(pixels, sprites[sprite_num], ntime);
}
