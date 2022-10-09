#include "gfx3d.h"

#define WIDTH 192
#define HEIGHT 192

static void blitfill(uint32_t *pixels, double *zbuffer, int offset, int len, double z1, double z2, uint32_t colour) {
    uint32_t *pixels_pos = pixels + offset;
    double *zbuffer_pos = zbuffer + offset;
    double z = z1;
    double zstep = (z2 - z1) / (double)len;

    for (int i = 0; i < len; i++) {
        if (z < *zbuffer_pos) {
            *pixels_pos = colour;
            *zbuffer_pos = z;
        }
        pixels_pos++;
        zbuffer_pos++;
    }
}

void gfx3d_flat_tri(uint32_t *pixels, double *zbuffer, vec3 v1, vec3 v2, vec3 v3, uint32_t colour) {
    /* find z component of cross product to test for back-face culling */
    double xpz = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);

    if (xpz > 0) return;

    int v1x = (int)(96 + 96 * v1.x);
    int v1y = (int)(96 - 96 * v1.y);
    double v1z = v1.z;
    int v2x = (int)(96 + 96 * v2.x);
    int v2y = (int)(96 - 96 * v2.y);
    double v2z = v2.z;
    int v3x = (int)(96 + 96 * v3.x);
    int v3y = (int)(96 - 96 * v3.y);
    double v3z = v3.z;

    int tempx, tempy;
    double tempz;
    int x1,y1,x2,y2,x3,y3;
    double z1,z2,z3;
    int xstep1,xstep2,xstep3;
    double zstep1,zstep2,zstep3;
    int dx1,dx2;
    int dy31,dy21,dy32;
    double dz1, dz2;
    int startx,endx,ry,py;


    // Sort points
    if (v1y > v2y) {
        tempx = v1x; tempy = v1y; tempz = v1z;
        v1x = v2x; v1y = v2y; v1z = v2z;
        v2x = tempx; v2y = tempy; v2z = tempz;
    }
    if (v1y > v3y) {
        tempx = v1x; tempy = v1y; tempz = v1z;
        v1x = v3x; v1y = v3y; v1z = v3z;
        v3x = tempx; v3y = tempy; v3z = tempz;
    }
    if (v2y > v3y) {
        tempx = v2x; tempy = v2y; tempz = v2z;
        v2x = v3x; v2y = v3y; v2z = v3z;
        v3x = tempx; v3y = tempy; v3z = tempz;
    }
    if ((v1y == v2y) && (v1x > v2x)) {
        x1 = v2x; x2 = v1x; x3 = v3x;
        z1 = v2z; z2 = v1z; z3 = v3z;
    } else {
        x1 = v1x; x2 = v2x; x3 = v3x;
        z1 = v1z; z2 = v2z; z3 = v3z;
    }
    y1 = v1y; y2 = v2y; y3 = v3y;

    // Calculate Edge steps
    dy31 = (y3-y1); dy21 = (y2-y1); dy32 = (y3-y2);
    xstep1 = (x3-x1) << 16;
    zstep1 = (z3-z1);
    if (dy31>0) {
        xstep1/=dy31;
        zstep1/=(double)dy31;
    }
    xstep2 = (x2-x1) << 16;
    zstep2 = (z2-z1);
    if (dy21>0) {
        xstep2/=dy21;
        zstep2/=(double)dy21;
    }
    xstep3 = (x3-x2) << 16;
    zstep3 = (z3-z2);
    if (dy32>0) {
        xstep3/=dy32;
        zstep3/=(double)dy32;
    }

    // Start drawing
    dx1 = x1 << 16; dx2 = dx1;
    dz1 = z1; dz2 = z1;
    py = y1 * WIDTH;

    // Long edge on the left
    if (xstep1 < xstep2) {
        for (ry=y1; ry<=y3; ry++) {
            startx = dx1 >> 16; endx = dx2 >> 16;
            if ((ry >= 0) && (ry < HEIGHT) && (startx < WIDTH) && (endx >= 0)) {
                if (startx < 0) startx = 0;
                if (endx > WIDTH - 1) endx = WIDTH - 1;
                if ((endx - startx + 1) > 0) {
                    blitfill(pixels, zbuffer, startx + py, endx - startx + 1, dz1, dz2, colour);
                }
            }
            dx1 += xstep1;
            dz1 += zstep1;
            if (ry < y2) {
                dx2 += xstep2; dz2 += zstep2;
            }
            if (ry == y2) {
                dx2 = x2 << 16; dz2 = z2;
            }
            if (ry > y2) {
                dx2 += xstep3; dz2 += zstep3;
            }
            py += WIDTH;
        }
    }

    // Long edge on the right
    if (xstep1 >= xstep2) {
        for (ry=y1; ry<=y3; ry++) {
            startx = dx1 >> 16; endx = dx2 >> 16;
            if ((ry >= 0) && (ry < HEIGHT) && (startx < WIDTH) && (endx >= 0)) {
                if (startx < 0) startx = 0;
                if (endx > WIDTH - 1) endx = WIDTH - 1;
                if ((endx - startx + 1) > 0) {
                    blitfill(pixels, zbuffer, startx + py, endx - startx + 1, dz1, dz2, colour);
                }
            }
            dx2 += xstep1;
            dz2 += zstep1;
            if (ry < y2) {
                dx1 += xstep2; dz1 += zstep2;
            }
            if (ry == y2) {
                dx1 = x2 << 16;
                dz1 = z2;
            }
            if (ry > y2) {
                dx1 += xstep3; dz1 += zstep3;
            }
            py += WIDTH;
        }
    }
}
