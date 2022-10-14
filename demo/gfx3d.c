#include "gfx3d.h"

#define WIDTH 192
#define HEIGHT 192

#include <math.h>

void mat4_identity(mat4 out) {
    out[0] = 1;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;
    out[4] = 0;
    out[5] = 1;
    out[6] = 0;
    out[7] = 0;
    out[8] = 0;
    out[9] = 0;
    out[10] = 1;
    out[11] = 0;
    out[12] = 0;
    out[13] = 0;
    out[14] = 0;
    out[15] = 1;
}

void mat4_ortho(mat4 out, double left, double right, double bottom, double top, double znear, double zfar) {
    out[0] = 2.0 / (right - left);
    out[1] = 0.0;
    out[2] = 0.0;
    out[3] = 0.0;

    out[4] = 0.0;
    out[5] = 2.0 / (top - bottom);
    out[6] = 0.0;
    out[7] = 0.0;

    out[8] = 0.0;
    out[9] = 0.0;
    out[10] = -2.0 / (zfar - znear);
    out[11] = 0.0;

    out[12] = -(right + left) / (right - left);
    out[13] = -(top + bottom) / (top - bottom);
    out[14] = -(zfar + znear) / (zfar - znear);
    out[15] = 1.0;
}

void mat4_mul(mat4 out, mat4 m1, mat4 m2) {
    out[0] = m1[0]*m2[0] + m1[1]*m2[4] + m1[2]*m2[8] + m1[3]*m2[12];
    out[1] = m1[0]*m2[1] + m1[1]*m2[5] + m1[2]*m2[9] + m1[3]*m2[13];
    out[2] = m1[0]*m2[2] + m1[1]*m2[6] + m1[2]*m2[10] + m1[3]*m2[14];
    out[3] = m1[0]*m2[3] + m1[1]*m2[7] + m1[2]*m2[11] + m1[3]*m2[15];

    out[4] = m1[4]*m2[0] + m1[5]*m2[4] + m1[6]*m2[8] + m1[7]*m2[12];
    out[5] = m1[4]*m2[1] + m1[5]*m2[5] + m1[6]*m2[9] + m1[7]*m2[13];
    out[6] = m1[4]*m2[2] + m1[5]*m2[6] + m1[6]*m2[10] + m1[7]*m2[14];
    out[7] = m1[4]*m2[3] + m1[5]*m2[7] + m1[6]*m2[11] + m1[7]*m2[15];

    out[8] = m1[8]*m2[0] + m1[9]*m2[4] + m1[10]*m2[8] + m1[11]*m2[12];
    out[9] = m1[8]*m2[1] + m1[9]*m2[5] + m1[10]*m2[9] + m1[11]*m2[13];
    out[10] = m1[8]*m2[2] + m1[9]*m2[6] + m1[10]*m2[10] + m1[11]*m2[14];
    out[11] = m1[8]*m2[3] + m1[9]*m2[7] + m1[10]*m2[11] + m1[11]*m2[15];

    out[12] = m1[12]*m2[0] + m1[13]*m2[4] + m1[14]*m2[8] + m1[15]*m2[12];
    out[13] = m1[12]*m2[1] + m1[13]*m2[5] + m1[14]*m2[9] + m1[15]*m2[13];
    out[14] = m1[12]*m2[2] + m1[13]*m2[6] + m1[14]*m2[10] + m1[15]*m2[14];
    out[15] = m1[12]*m2[3] + m1[13]*m2[7] + m1[14]*m2[11] + m1[15]*m2[15];
}

vec3 mat4_mul_vec3(vec3 vec, mat4 mat) {
    double w = vec.x*mat[12] + vec.y*mat[13] + vec.z*mat[14] + mat[15];
    vec3 out = {
        (vec.x*mat[0] + vec.y*mat[1] + vec.z*mat[2] + mat[3])/w,
        (vec.x*mat[4] + vec.y*mat[5] + vec.z*mat[6] + mat[7])/w,
        (vec.x*mat[8] + vec.y*mat[9] + vec.z*mat[10] + mat[11])/w
    };
    return out;
}

vec3 mat3_mul_vec3(vec3 vec, mat3 mat) {
    vec3 out = {
        (vec.x*mat[0] + vec.y*mat[1] + vec.z*mat[2]),
        (vec.x*mat[3] + vec.y*mat[4] + vec.z*mat[5]),
        (vec.x*mat[6] + vec.y*mat[7] + vec.z*mat[8])
    };
    return out;
}

void mat4_rotate_x(mat4 mat, double angle) {
    double s = sin(angle);
    double c = cos(angle);
    double a10 = mat[4];
    double a11 = mat[5];
    double a12 = mat[6];
    double a13 = mat[7];
    double a20 = mat[8];
    double a21 = mat[9];
    double a22 = mat[10];
    double a23 = mat[11];

    mat[4] = a10 * c + a20 * s;
    mat[5] = a11 * c + a21 * s;
    mat[6] = a12 * c + a22 * s;
    mat[7] = a13 * c + a23 * s;

    mat[8] = a10 * -s + a20 * c;
    mat[9] = a11 * -s + a21 * c;
    mat[10] = a12 * -s + a22 * c;
    mat[11] = a13 * -s + a23 * c;
}

void mat4_rotate_y(mat4 mat, double angle) {
    double s = sin(angle);
    double c = cos(angle);
    double a00 = mat[0];
    double a01 = mat[1];
    double a02 = mat[2];
    double a03 = mat[3];
    double a20 = mat[8];
    double a21 = mat[9];
    double a22 = mat[10];
    double a23 = mat[11];

    mat[0] = a00 * c + a20 * -s;
    mat[1] = a01 * c + a21 * -s;
    mat[2] = a02 * c + a22 * -s;
    mat[3] = a03 * c + a23 * -s;

    mat[8] = a00 * s + a20 * c;
    mat[9] = a01 * s + a21 * c;
    mat[10] = a02 * s + a22 * c;
    mat[11] = a03 * s + a23 * c;
}

void mat4_translate(mat4 mat, vec3 vec) {
    mat[3] += vec.x;
    mat[7] += vec.y;
    mat[11] += vec.z;
}

void mat4_to_inverse_transpose_mat3(mat4 mat, mat3 dest) {
    /*
        Calculates the inverse of the upper 3x3 elements of a mat4 and copies the result into a mat3
        The resulting matrix is useful for calculating transformed normals
    */
    double a00 = mat[0];
    double a01 = mat[1];
    double a02 = mat[2];

    double a10 = mat[4];
    double a11 = mat[5];
    double a12 = mat[6];

    double a20 = mat[8];
    double a21 = mat[9];
    double a22 = mat[10];

    double b01 = a22 * a11 - a12 * a21;
    double b11 = -a22 * a10 + a12 * a20;
    double b21 = a21 * a10 - a11 * a20;

    double d = a00 * b01 + a01 * b11 + a02 * b21;

    double id = 1.0 / d;

    dest[0] = b01 * id;
    dest[3] = (-a22 * a01 + a02 * a21) * id;
    dest[6] = (a12 * a01 - a02 * a11) * id;
    dest[1] = b11 * id;
    dest[4] = (a22 * a00 - a02 * a20) * id;
    dest[7] = (-a12 * a00 + a02 * a10) * id;
    dest[2] = b21 * id;
    dest[5] = (-a21 * a00 + a01 * a20) * id;
    dest[8] = (a11 * a00 - a01 * a10) * id;
}

vec3 vec3_normalize(vec3 vec) {
    double len = sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
    vec3 result = {vec.x/len, vec.y/len, vec.z/len};
    return result;
}

double vec3_dot(vec3 v1, vec3 v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}


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

static void blitfill_gouraud(uint32_t *pixels, double *zbuffer, int offset, int len, double z1, double z2, double bright1, double bright2) {
    uint32_t *pixels_pos = pixels + offset;
    double *zbuffer_pos = zbuffer + offset;
    double z = z1;
    double zstep = (z2 - z1) / (double)len;

    double bright = bright1;
    double brightstep = (bright2 - bright1) / (double)len;

    for (int i = 0; i < len; i++) {
        if (z < *zbuffer_pos) {
            *pixels_pos = (uint32_t)(bright * 200) * 0x01010100;
            *zbuffer_pos = z;
        }
        pixels_pos++;
        zbuffer_pos++;
        z += zstep;
        bright += brightstep;
    }
}

void gfx3d_gouraud_tri(uint32_t *pixels, double *zbuffer, vertex_out_attrs va1, vertex_out_attrs va2, vertex_out_attrs va3) {
    vec3 v1 = va1.position;
    vec3 v2 = va2.position;
    vec3 v3 = va3.position;
    /* find z component of cross product to test for back-face culling */
    double xpz = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);

    if (xpz > 0) return;

    int v1x = (int)(96 + 96 * v1.x);
    int v1y = (int)(96 - 96 * v1.y);
    int v2x = (int)(96 + 96 * v2.x);
    int v2y = (int)(96 - 96 * v2.y);
    int v3x = (int)(96 + 96 * v3.x);
    int v3y = (int)(96 - 96 * v3.y);

    int tempx, tempy;
    vertex_out_attrs tempva, a1, a2, a3;
    int x1,y1,x2,y2,x3,y3;
    int xstep1,xstep2,xstep3;
    double zstep1,zstep2,zstep3;
    double bstep1,bstep2,bstep3;
    int dx1,dx2;
    int dy31,dy21,dy32;
    double dz1,dz2;
    double db1,db2;
    int startx,endx,ry,py;


    // Sort points
    if (v1y > v2y) {
        tempx = v1x; tempy = v1y; tempva = va1;
        v1x = v2x; v1y = v2y; va1 = va2;
        v2x = tempx; v2y = tempy; va2 = tempva;
    }
    if (v1y > v3y) {
        tempx = v1x; tempy = v1y; tempva = va1;
        v1x = v3x; v1y = v3y; va1 = va3;
        v3x = tempx; v3y = tempy; va3 = tempva;
    }
    if (v2y > v3y) {
        tempx = v2x; tempy = v2y; tempva = va2;
        v2x = v3x; v2y = v3y; va2 = va3;
        v3x = tempx; v3y = tempy; va3 = tempva;
    }
    if ((v1y == v2y) && (v1x > v2x)) {
        x1 = v2x; x2 = v1x; x3 = v3x;
        tempva = va2; va2 = va1; va1 = tempva;
    } else {
        x1 = v1x; x2 = v2x; x3 = v3x;
    }
    y1 = v1y; y2 = v2y; y3 = v3y;

    // Calculate Edge steps
    dy31 = (y3-y1); dy21 = (y2-y1); dy32 = (y3-y2);
    xstep1 = (x3-x1) << 16;
    zstep1 = (va3.position.z-va1.position.z);
    bstep1 = (va3.brightness-va1.brightness);
    if (dy31>0) {
        xstep1/=dy31;
        zstep1/=(double)dy31;
        bstep1/=(double)dy31;
    }
    xstep2 = (x2-x1) << 16;
    zstep2 = (va2.position.z-va1.position.z);
    bstep2 = (va2.brightness-va1.brightness);
    if (dy21>0) {
        xstep2/=dy21;
        zstep2/=(double)dy21;
        bstep2/=(double)dy21;
    }
    xstep3 = (x3-x2) << 16;
    zstep3 = (va3.position.z-va2.position.z);
    bstep3 = (va3.brightness - va2.brightness);
    if (dy32>0) {
        xstep3/=dy32;
        zstep3/=(double)dy32;
        bstep3/=(double)dy32;
    }

    // Start drawing
    dx1 = x1 << 16; dx2 = dx1;
    dz1 = va1.position.z; dz2 = va1.position.z;
    db1 = va1.brightness; db2 = va1.brightness;
    py = y1 * 192;

    // Long edge on the left
    if (xstep1 < xstep2) {
        for (ry=y1; ry<=y3; ry++) {
            startx = dx1 >> 16; endx = dx2 >> 16;
            if ((ry >= 0) && (ry < 192) && (startx < 192) && (endx >= 0)) {
                if (startx < 0) startx = 0;
                if (endx > 192 - 1) endx = 192 - 1;
                if ((endx - startx + 1) > 0) {
                    blitfill_gouraud(pixels, zbuffer, startx + py, endx - startx + 1, dz1, dz2, db1, db2);
                }
            }
            dx1 += xstep1;
            dz1 += zstep1;
            db1 += bstep1;
            if (ry < y2) {
                dx2 += xstep2; dz2 += zstep2; db2 += bstep2;
            }
            if (ry == y2) {
                dx2 = x2 << 16; dz2 = va2.position.z; db2 = va2.brightness;
            }
            if (ry > y2) {
                dx2 += xstep3; dz2 += zstep3; db2 += bstep3;
            }
            py += 192;
        }
    }

    // Long edge on the right
    if (xstep1 >= xstep2) {
        for (ry=y1; ry<=y3; ry++) {
            startx = dx1 >> 16; endx = dx2 >> 16;
            if ((ry >= 0) && (ry < 192) && (startx < 192) && (endx >= 0)) {
                if (startx < 0) startx = 0;
                if (endx > 192 - 1) endx = 192 - 1;
                if ((endx - startx + 1) > 0) {
                    blitfill_gouraud(pixels, zbuffer, startx + py, endx - startx + 1, dz1, dz2, db1, db2);
                }
            }
            dx2 += xstep1;
            dz2 += zstep1;
            db2 += bstep1;
            if (ry < y2) {
                dx1 += xstep2; dz1 += zstep2; db1 += bstep2;
            }
            if (ry == y2) {
                dx1 = x2 << 16;
                dz1 = va2.position.z;
                db1 = va2.brightness;
            }
            if (ry > y2) {
                dx1 += xstep3; dz1 += zstep3; db1 += bstep3;
            }
            py += 192;
        }
    }
}

static void blitfill_tex(uint32_t *pixels, double *zbuffer, gfx_image *texture, int offset, int len, double z1, double z2, double bright1, double bright2, int u1, int u2, int v1, int v2) {
    uint32_t *pixels_pos = pixels + offset;
    double *zbuffer_pos = zbuffer + offset;
    double z = z1;
    double zstep = (z2 - z1) / (double)len;
    int u = u1;
    int ustep = (u2 - u1) / len;
    int v = v1;
    int vstep = (v2 - v1) / len;

    double bright = bright1;
    double brightstep = (bright2 - bright1) / (double)len;

    for (int i = 0; i < len; i++) {
        if (z < *zbuffer_pos) {
            unsigned char *texel = texture->data + ((v>>16)*texture->width + (u>>16)) * 4;
            *pixels_pos = (
                (texel[0] << 24)
                | (texel[1] << 16)
                | (texel[2] << 8)
            );
            *zbuffer_pos = z;
        }
        pixels_pos++;
        zbuffer_pos++;
        z += zstep;
        u += ustep;
        v += vstep;
        bright += brightstep;
    }
}

void gfx3d_gouraud_tex_tri(uint32_t *pixels, double *zbuffer, gfx_image *texture, vertex_out_attrs va1, vertex_out_attrs va2, vertex_out_attrs va3) {
    vec3 v1 = va1.position;
    vec3 v2 = va2.position;
    vec3 v3 = va3.position;
    /* find z component of cross product to test for back-face culling */
    double xpz = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);

    if (xpz > 0) return;

    int v1x = (int)(96 + 96 * v1.x);
    int v1y = (int)(96 - 96 * v1.y);
    int v2x = (int)(96 + 96 * v2.x);
    int v2y = (int)(96 - 96 * v2.y);
    int v3x = (int)(96 + 96 * v3.x);
    int v3y = (int)(96 - 96 * v3.y);

    int tempx, tempy;
    vertex_out_attrs tempva, a1, a2, a3;
    int x1,y1,x2,y2,x3,y3;
    int xstep1,xstep2,xstep3;
    double zstep1,zstep2,zstep3;
    double bstep1,bstep2,bstep3;
    int ustep1,ustep2,ustep3;
    int vstep1,vstep2,vstep3;
    int dx1,dx2;
    int dy31,dy21,dy32;
    double dz1,dz2;
    double db1,db2;
    int du1,du2;
    int dv1,dv2;
    int startx,endx,ry,py;


    // Sort points
    if (v1y > v2y) {
        tempx = v1x; tempy = v1y; tempva = va1;
        v1x = v2x; v1y = v2y; va1 = va2;
        v2x = tempx; v2y = tempy; va2 = tempva;
    }
    if (v1y > v3y) {
        tempx = v1x; tempy = v1y; tempva = va1;
        v1x = v3x; v1y = v3y; va1 = va3;
        v3x = tempx; v3y = tempy; va3 = tempva;
    }
    if (v2y > v3y) {
        tempx = v2x; tempy = v2y; tempva = va2;
        v2x = v3x; v2y = v3y; va2 = va3;
        v3x = tempx; v3y = tempy; va3 = tempva;
    }
    if ((v1y == v2y) && (v1x > v2x)) {
        x1 = v2x; x2 = v1x; x3 = v3x;
        tempva = va2; va2 = va1; va1 = tempva;
    } else {
        x1 = v1x; x2 = v2x; x3 = v3x;
    }
    y1 = v1y; y2 = v2y; y3 = v3y;

    // Calculate Edge steps
    dy31 = (y3-y1); dy21 = (y2-y1); dy32 = (y3-y2);
    xstep1 = (x3-x1) << 16;
    zstep1 = (va3.position.z-va1.position.z);
    bstep1 = (va3.brightness-va1.brightness);
    ustep1 = (va3.u-va1.u) << 16;
    vstep1 = (va3.v-va1.v) << 16;
    if (dy31>0) {
        xstep1/=dy31;
        zstep1/=(double)dy31;
        bstep1/=(double)dy31;
        ustep1/=dy31;
        vstep1/=dy31;
    }
    xstep2 = (x2-x1) << 16;
    zstep2 = (va2.position.z-va1.position.z);
    bstep2 = (va2.brightness-va1.brightness);
    ustep2 = (va2.u-va1.u) << 16;
    vstep2 = (va2.v-va1.v) << 16;
    if (dy21>0) {
        xstep2/=dy21;
        zstep2/=(double)dy21;
        bstep2/=(double)dy21;
        ustep2/=dy21;
        vstep2/=dy21;
    }
    xstep3 = (x3-x2) << 16;
    zstep3 = (va3.position.z-va2.position.z);
    bstep3 = (va3.brightness - va2.brightness);
    ustep3 = (va3.u-va2.u) << 16;
    vstep3 = (va3.v-va2.v) << 16;
    if (dy32>0) {
        xstep3/=dy32;
        zstep3/=(double)dy32;
        bstep3/=(double)dy32;
        ustep3/=dy32;
        vstep3/=dy32;
    }

    // Start drawing
    dx1 = x1 << 16; dx2 = dx1;
    dz1 = va1.position.z; dz2 = va1.position.z;
    db1 = va1.brightness; db2 = va1.brightness;
    du1 = va1.u; du2 = va1.u;
    dv1 = va1.v; dv2 = va1.v;
    py = y1 * 192;

    // Long edge on the left
    if (xstep1 < xstep2) {
        for (ry=y1; ry<=y3; ry++) {
            startx = dx1 >> 16; endx = dx2 >> 16;
            if ((ry >= 0) && (ry < 192) && (startx < 192) && (endx >= 0)) {
                if (startx < 0) startx = 0;
                if (endx > 192 - 1) endx = 192 - 1;
                if ((endx - startx + 1) > 0) {
                    blitfill_tex(pixels, zbuffer, texture, startx + py, endx - startx + 1, dz1, dz2, db1, db2, du1, du2, dv1, dv2);
                }
            }
            dx1 += xstep1;
            dz1 += zstep1;
            db1 += bstep1;
            du1 += ustep1;
            dv1 += vstep1;
            if (ry < y2) {
                dx2 += xstep2; dz2 += zstep2; db2 += bstep2;
                du2 += ustep2; dv2 += vstep2;
            }
            if (ry == y2) {
                dx2 = x2 << 16; dz2 = va2.position.z; db2 = va2.brightness;
                du2 = va2.u << 16; dv2 = va2.v << 16;
            }
            if (ry > y2) {
                dx2 += xstep3; dz2 += zstep3; db2 += bstep3;
                du2 += ustep3; dv2 += vstep3;
            }
            py += 192;
        }
    }

    // Long edge on the right
    if (xstep1 >= xstep2) {
        for (ry=y1; ry<=y3; ry++) {
            startx = dx1 >> 16; endx = dx2 >> 16;
            if ((ry >= 0) && (ry < 192) && (startx < 192) && (endx >= 0)) {
                if (startx < 0) startx = 0;
                if (endx > 192 - 1) endx = 192 - 1;
                if ((endx - startx + 1) > 0) {
                    blitfill_tex(pixels, zbuffer, texture, startx + py, endx - startx + 1, dz1, dz2, db1, db2, du1, du2, dv1, dv2);
                }
            }
            dx2 += xstep1;
            dz2 += zstep1;
            db2 += bstep1;
            du2 += ustep1;
            dv2 += vstep1;
            if (ry < y2) {
                dx1 += xstep2; dz1 += zstep2; db1 += bstep2;
                du1 += ustep2; dv1 += vstep2;
            }
            if (ry == y2) {
                dx1 = x2 << 16;
                dz1 = va2.position.z;
                db1 = va2.brightness;
                du1 = va2.u << 16; dv1 = va2.v << 16;
            }
            if (ry > y2) {
                dx1 += xstep3; dz1 += zstep3; db1 += bstep3;
                du1 += ustep3; dv1 += vstep3;
            }
            py += 192;
        }
    }
}
