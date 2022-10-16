#include "gfx3d.h"

#define WIDTH 192
#define HEIGHT 192

#include <stdio.h>
#include <stdlib.h>
#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags


void gfx3d_load_model(char *model_filename, char *texture_filename, gfx3d_model *model) {
    const struct aiMesh *mesh;

    int has_texture = 0;
    if (texture_filename != NULL) {
        has_texture = 1;
        gfx_loadimage(texture_filename, &model->texture);
    }

    // Start the import on the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll t
    // probably to request more postprocessing than we do in this example.
    const struct aiScene* scene = aiImportFile(model_filename,
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_GenSmoothNormals       |
        aiProcess_SortByPType);

    if (scene != NULL) {
        mesh = scene->mMeshes[0];
        model->vertex_count = mesh->mNumVertices;
        model->face_count = mesh->mNumFaces;

        model->vertices = malloc(model->vertex_count * sizeof(vertex_in_attrs));
        if (model->vertices == NULL) {
            printf("could not allocate vertices\n");
        }

        struct aiVector3D *texture_coords = mesh->mTextureCoords[0];

        for (unsigned int i = 0; i < model->vertex_count; i++) {
            struct aiVector3D v = mesh->mVertices[i];
            model->vertices[i].position.x = v.x;
            model->vertices[i].position.y = v.y;
            model->vertices[i].position.z = v.z;
            struct aiVector3D n = mesh->mNormals[i];
            model->vertices[i].normal.x = n.x;
            model->vertices[i].normal.y = n.y;
            model->vertices[i].normal.z = n.z;
            if (has_texture && texture_coords != NULL) {
                struct aiVector3D t = texture_coords[i];
                model->vertices[i].u = (int)(t.x * model->texture.width);
                model->vertices[i].v = (int)((1 - t.y) * model->texture.height);
            }
        }

        model->transformed_vertices = malloc(model->vertex_count * sizeof(vertex_out_attrs));
        if (model->transformed_vertices == NULL) {
            printf("could not allocate transformed_vertices\n");
        }

        model->faces = malloc(model->face_count * sizeof(gfx3d_face));
        if (model->faces == NULL) {
            printf("could not allocate faces\n");
        }
        for (unsigned int i = 0; i < model->face_count; i++) {
            struct aiFace face = mesh->mFaces[i];
            model->faces[i].index1 = face.mIndices[0];
            model->faces[i].index2 = face.mIndices[1];
            model->faces[i].index3 = face.mIndices[2];
        }
    } else {
        printf("obj import failed: %s\n", aiGetErrorString());
    }
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
            int bright_int = (int)(bright * 255);
            if (bright_int > 255) bright_int = 255;
            *pixels_pos = (
                ((texel[0] * bright_int & 0xff00) << 16)
                | ((texel[1] * bright_int & 0xff00) << 8)
                | (texel[2] * bright_int & 0xff00)
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
    du1 = va1.u << 16; du2 = du1;
    dv1 = va1.v << 16; dv2 = dv1;
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
