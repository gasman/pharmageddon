#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags
#include "gfx.h"
#include "gfx3d.h"
#include "ambulance.h"

typedef struct vertex_in_attrs {
    vec3 position;
    vec3 normal;
} vertex_in_attrs;

typedef struct vertex_out_attrs {
    vec3 position;
    double brightness;
} vertex_out_attrs;

const struct aiMesh *ambulance;
unsigned int vertex_count;
unsigned int face_count;

double zbuffer[192*192];

vertex_in_attrs *model_vertices;
vertex_out_attrs *transformed_vertices;

void ambulance_init(void) {
    // Start the import on the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll t
    // probably to request more postprocessing than we do in this example.
    const struct aiScene* scene = aiImportFile( "../assets/ambulance.fbx",
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_GenSmoothNormals       |
        aiProcess_SortByPType);

    if (scene != NULL) {
        ambulance = scene->mMeshes[0];
        vertex_count = ambulance->mNumVertices;
        face_count = ambulance->mNumFaces;

        model_vertices = malloc(vertex_count * sizeof(vertex_in_attrs));
        if (model_vertices == NULL) {
            printf("could not allocate model_vertices\n");
        }

        for (unsigned int i = 0; i < vertex_count; i++) {
            struct aiVector3D v = ambulance->mVertices[i];
            model_vertices[i].position.x = v.x;
            model_vertices[i].position.y = v.y;
            model_vertices[i].position.z = v.z;
            struct aiVector3D n = ambulance->mNormals[i];
            model_vertices[i].normal.x = n.x;
            model_vertices[i].normal.y = n.y;
            model_vertices[i].normal.z = n.z;
        }

        transformed_vertices = malloc(vertex_count * sizeof(vertex_out_attrs));
        if (transformed_vertices == NULL) {
            printf("could not allocate transformed_vertices\n");
        }
    } else {
        printf("obj import failed: %s\n", aiGetErrorString());
    }
}

static void blitfill(uint32_t *pixels, double *zbuffer, int offset, int len, double z1, double z2, double bright1, double bright2) {
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

static void gfx3d_gouraud_tri(uint32_t *pixels, double *zbuffer, vertex_out_attrs va1, vertex_out_attrs va2, vertex_out_attrs va3) {
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
                    blitfill(pixels, zbuffer, startx + py, endx - startx + 1, dz1, dz2, db1, db2);
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
                    blitfill(pixels, zbuffer, startx + py, endx - startx + 1, dz1, dz2, db1, db2);
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


void ambulance_frame(uint32_t *pixels, uint32_t time) {
    mat4 rotate_matrix;
    mat3 normal_rotate_matrix;
    // mat4 projection_matrix;

    gfx_cls(pixels, 0x00110000);

    mat4_identity(rotate_matrix);
    vec3 trans1 = {0, -2, 1};
    mat4_translate(rotate_matrix, trans1);
    mat4_rotate_y(rotate_matrix, ((double)time) / 1000);
    // mat4_rotate_x(rotate_matrix, ((double)time) / 900);
    // mat4_ortho( projection_matrix, -5.0, 5.0, 5.0, -5.0, -30.0, 100.0 );
    vec3 trans2 = {0, 0, 7};
    mat4_translate(rotate_matrix, trans2);
    mat4_to_inverse_transpose_mat3(rotate_matrix, normal_rotate_matrix);

    vec3 light_pos = {0, 10, -2};

    for (unsigned int i = 0; i < vertex_count; i++) {
        vec3 pos = mat4_mul_vec3(model_vertices[i].position, rotate_matrix);
        vec3 normal = mat3_mul_vec3(vec3_normalize(model_vertices[i].normal), normal_rotate_matrix);
        // vec3 normal = model_vertices[i].normal;
        vec3 light_dir = {light_pos.x - pos.x, light_pos.y - pos.y, light_pos.z - pos.z};
        light_dir = vec3_normalize(light_dir);
        double diffuse = vec3_dot(normal, light_dir);
        if (diffuse < 0) diffuse = 0;

        // hacky perspective transform
        vec3 projected_pos = {pos.x/pos.z, pos.y/pos.z, pos.z};
        transformed_vertices[i].position = projected_pos;

        transformed_vertices[i].brightness = diffuse;
    }

    for (double *zbuffer_ptr = zbuffer; zbuffer_ptr < (zbuffer + 192*192); zbuffer_ptr++) {
        *zbuffer_ptr = 100000;
    }

    for (unsigned int i = 0; i < face_count; i++) {
        struct aiFace face = ambulance->mFaces[i];
        vertex_out_attrs va0 = transformed_vertices[face.mIndices[0]];
        vertex_out_attrs va1 = transformed_vertices[face.mIndices[1]];
        vertex_out_attrs va2 = transformed_vertices[face.mIndices[2]];

        gfx3d_gouraud_tri(
            pixels, zbuffer, va0, va1, va2
        );
    }
}
