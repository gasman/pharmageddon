#include <stdint.h>

void gfx_cls(uint32_t *pixels, uint32_t colour);
void gfx_putpixel(uint32_t *pixels, int x, int y, uint32_t colour);
void gfx_fillrect(uint32_t *pixels, int x, int y, int w, int h, uint32_t colour);
void gfx_line(uint32_t *pixels, int x1, int y1, int x2, int y2, uint32_t colour);
