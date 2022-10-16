#ifndef GFX_H
#define GFX_H

#include <stdint.h>

void gfx_cls(uint32_t *pixels, uint32_t colour);
void gfx_putpixel(uint32_t *pixels, int x, int y, uint32_t colour);
void gfx_rect(uint32_t *pixels, int x, int y, int w, int h, uint32_t colour);
void gfx_fillrect(uint32_t *pixels, int x, int y, int w, int h, uint32_t colour);
void gfx_line(uint32_t *pixels, int x1, int y1, int x2, int y2, uint32_t colour);
void gfx_hline(uint32_t *pixels, int x1, int x2, int y, uint32_t colour);
void gfx_circle(uint32_t *pixels, int cx, int cy, int r, uint32_t colour);
void gfx_fillcircle(uint32_t *pixels, int cx, int cy, int r, uint32_t colour);
void gfx_tri(uint32_t *pixels, int v1x, int v1y, int v2x, int v2y, int v3x, int v3y, uint32_t colour);
void gfx_filltri(uint32_t *pixels, int v1x, int v1y, int v2x, int v2y, int v3x, int v3y, uint32_t colour);

typedef struct gfx_image {
    unsigned char *data;
    int width;
    int height;
} gfx_image;

void gfx_loadimage(char *filename, gfx_image *image);
void gfx_drawimage(uint32_t *pixels, gfx_image *image, int x, int y);
void gfx_rotateimage(uint32_t *pixels, gfx_image *image, double cx, double cy, double r, double s);

#endif /* GFX_H */
