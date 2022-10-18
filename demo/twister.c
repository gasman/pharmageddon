#include <stdlib.h>
#include <math.h>
#include "gfx3d.h"
#include "twister.h"

double zbuffer[192*192];

gfx3d_model square;

#define SQUARE_VERTEX_COUNT 24
#define SQUARE_FACE_COUNT 12

#define SLICE_HEIGHT 0.1

static vertex_in_attrs square_vertices[SQUARE_VERTEX_COUNT] = {
    /* top */
    { {-1, SLICE_HEIGHT, -1}, {0, 1, 0}, 0, 0 },
    { {-1, SLICE_HEIGHT, 1}, {0, 1, 0}, 0, 0 },
    { {1, SLICE_HEIGHT, 1}, {0, 1, 0}, 0, 0 },
    { {1, SLICE_HEIGHT, -1}, {0, 1, 0}, 0, 0 },

    /* bottom */
    { {1, -SLICE_HEIGHT, -1}, {0, -1, 0}, 0, 0 },
    { {1, -SLICE_HEIGHT, 1}, {0, -1, 0}, 0, 0 },
    { {-1, -SLICE_HEIGHT, 1}, {0, -1, 0}, 0, 0 },
    { {-1, -SLICE_HEIGHT, -1}, {0, -1, 0}, 0, 0 },

    /* front */
    { {-1, SLICE_HEIGHT, -1}, {0, 0, -1}, 0, 0 },
    { {1, SLICE_HEIGHT, -1}, {0, 0, -1}, 0, 0 },
    { {1, -SLICE_HEIGHT, -1}, {0, 0, -1}, 0, 0 },
    { {-1, -SLICE_HEIGHT, -1}, {0, 0, -1}, 0, 0 },

    /* left */
    { {-1, SLICE_HEIGHT, 1}, {-1, 0, 0}, 0, 0 },
    { {-1, SLICE_HEIGHT, -1}, {-1, 0, 0}, 0, 0 },
    { {-1, -SLICE_HEIGHT, -1}, {-1, 0, 0}, 0, 0 },
    { {-1, -SLICE_HEIGHT, 1}, {-1, 0, 0}, 0, 0 },

    /* back */
    { {1, SLICE_HEIGHT, 1}, {0, 0, 1}, 0, 0 },
    { {-1, SLICE_HEIGHT, 1}, {0, 0, 1}, 0, 0 },
    { {-1, -SLICE_HEIGHT, 1}, {0, 0, 1}, 0, 0 },
    { {1, -SLICE_HEIGHT, 1}, {0, 0, 1}, 0, 0 },

    /* right */
    { {1, SLICE_HEIGHT, -1}, {1, 0, 0}, 0, 0 },
    { {1, SLICE_HEIGHT, 1}, {1, 0, 0}, 0, 0 },
    { {1, -SLICE_HEIGHT, 1}, {1, 0, 0}, 0, 0 },
    { {1, -SLICE_HEIGHT, -1}, {1, 0, 0}, 0, 0 }
};
static vertex_out_attrs square_transformed_vertices[SQUARE_VERTEX_COUNT];
static gfx3d_face square_faces[SQUARE_FACE_COUNT] = {
    {0, 1, 2}, {0, 2, 3},
    {4, 5, 6}, {4, 6, 7},
    {8, 9, 10}, {8, 10, 11},
    {12, 13, 14}, {12, 14, 15},
    {16, 17, 18}, {16, 18, 19},
    {20, 21, 22}, {20, 22, 23}
};


gfx3d_model cross;

#define CROSS_VERTEX_COUNT 88
#define CROSS_FACE_COUNT 44

static vertex_in_attrs cross_vertices[CROSS_VERTEX_COUNT] = {
    /* top of back square */
    { {-1, SLICE_HEIGHT, 1}, {0, 1, 0}, 0, 0 },
    { {-1, SLICE_HEIGHT, 3}, {0, 1, 0}, 0, 0 },
    { {1, SLICE_HEIGHT, 3}, {0, 1, 0}, 0, 0 },
    { {1, SLICE_HEIGHT, 1}, {0, 1, 0}, 0, 0 },

    /* top of left square */
    { {-3, SLICE_HEIGHT, -1}, {0, 1, 0}, 0, 0 },
    { {-3, SLICE_HEIGHT, 1}, {0, 1, 0}, 0, 0 },
    { {-1, SLICE_HEIGHT, 1}, {0, 1, 0}, 0, 0 },
    { {-1, SLICE_HEIGHT, -1}, {0, 1, 0}, 0, 0 },

    /* top of centre square */
    { {-1, SLICE_HEIGHT, -1}, {0, 1, 0}, 0, 0 },
    { {-1, SLICE_HEIGHT, 1}, {0, 1, 0}, 0, 0 },
    { {1, SLICE_HEIGHT, 1}, {0, 1, 0}, 0, 0 },
    { {1, SLICE_HEIGHT, -1}, {0, 1, 0}, 0, 0 },

    /* top of right square */
    { {1, SLICE_HEIGHT, -1}, {0, 1, 0}, 0, 0 },
    { {1, SLICE_HEIGHT, 1}, {0, 1, 0}, 0, 0 },
    { {3, SLICE_HEIGHT, 1}, {0, 1, 0}, 0, 0 },
    { {3, SLICE_HEIGHT, -1}, {0, 1, 0}, 0, 0 },

    /* top of front square */
    { {-1, SLICE_HEIGHT, -3}, {0, 1, 0}, 0, 0 },
    { {-1, SLICE_HEIGHT, -1}, {0, 1, 0}, 0, 0 },
    { {1, SLICE_HEIGHT, -1}, {0, 1, 0}, 0, 0 },
    { {1, SLICE_HEIGHT, -3}, {0, 1, 0}, 0, 0 },

    /* bottom of back square */
    { {1, -SLICE_HEIGHT, 1}, {0, -1, 0}, 0, 0 },
    { {1, -SLICE_HEIGHT, 3}, {0, -1, 0}, 0, 0 },
    { {-1, -SLICE_HEIGHT, 3}, {0, -1, 0}, 0, 0 },
    { {-1, -SLICE_HEIGHT, 1}, {0, -1, 0}, 0, 0 },

    /* bottom of left square */
    { {-1, -SLICE_HEIGHT, -1}, {0, -1, 0}, 0, 0 },
    { {-1, -SLICE_HEIGHT, 1}, {0, -1, 0}, 0, 0 },
    { {-3, -SLICE_HEIGHT, 1}, {0, -1, 0}, 0, 0 },
    { {-3, -SLICE_HEIGHT, -1}, {0, -1, 0}, 0, 0 },

    /* bottom of centre square */
    { {1, -SLICE_HEIGHT, -1}, {0, -1, 0}, 0, 0 },
    { {1, -SLICE_HEIGHT, 1}, {0, -1, 0}, 0, 0 },
    { {-1, -SLICE_HEIGHT, 1}, {0, -1, 0}, 0, 0 },
    { {-1, -SLICE_HEIGHT, -1}, {0, -1, 0}, 0, 0 },

    /* bottom of right square */
    { {3, -SLICE_HEIGHT, -1}, {0, -1, 0}, 0, 0 },
    { {3, -SLICE_HEIGHT, 1}, {0, -1, 0}, 0, 0 },
    { {1, -SLICE_HEIGHT, 1}, {0, -1, 0}, 0, 0 },
    { {1, -SLICE_HEIGHT, -1}, {0, -1, 0}, 0, 0 },

    /* bottom of front square */
    { {1, -SLICE_HEIGHT, -3}, {0, -1, 0}, 0, 0 },
    { {1, -SLICE_HEIGHT, -1}, {0, -1, 0}, 0, 0 },
    { {-1, -SLICE_HEIGHT, -1}, {0, -1, 0}, 0, 0 },
    { {-1, -SLICE_HEIGHT, -3}, {0, -1, 0}, 0, 0 },

    /* front of front square */
    { {-1, SLICE_HEIGHT, -3}, {0, 0, -1}, 0, 0 },
    { {1, SLICE_HEIGHT, -3}, {0, 0, -1}, 0, 0 },
    { {1, -SLICE_HEIGHT, -3}, {0, 0, -1}, 0, 0 },
    { {-1, -SLICE_HEIGHT, -3}, {0, 0, -1}, 0, 0 },

    /* right of front square */
    { {1, SLICE_HEIGHT, -3}, {1, 0, 0}, 0, 0 },
    { {1, SLICE_HEIGHT, -1}, {1, 0, 0}, 0, 0 },
    { {1, -SLICE_HEIGHT, -1}, {1, 0, 0}, 0, 0 },
    { {1, -SLICE_HEIGHT, -3}, {1, 0, 0}, 0, 0 },

    /* front of right square */
    { {1, SLICE_HEIGHT, -1}, {0, 0, -1}, 0, 0 },
    { {3, SLICE_HEIGHT, -1}, {0, 0, -1}, 0, 0 },
    { {3, -SLICE_HEIGHT, -1}, {0, 0, -1}, 0, 0 },
    { {1, -SLICE_HEIGHT, -1}, {0, 0, -1}, 0, 0 },

    /* right of right square */
    { {3, SLICE_HEIGHT, -1}, {1, 0, 0}, 0, 0 },
    { {3, SLICE_HEIGHT, 1}, {1, 0, 0}, 0, 0 },
    { {3, -SLICE_HEIGHT, 1}, {1, 0, 0}, 0, 0 },
    { {3, -SLICE_HEIGHT, -1}, {1, 0, 0}, 0, 0 },

    /* back of right square */
    { {3, SLICE_HEIGHT, 1}, {0, 0, 1}, 0, 0 },
    { {1, SLICE_HEIGHT, 1}, {0, 0, 1}, 0, 0 },
    { {1, -SLICE_HEIGHT, 1}, {0, 0, 1}, 0, 0 },
    { {3, -SLICE_HEIGHT, 1}, {0, 0, 1}, 0, 0 },

    /* right of back square */
    { {1, SLICE_HEIGHT, 1}, {1, 0, 0}, 0, 0 },
    { {1, SLICE_HEIGHT, 3}, {1, 0, 0}, 0, 0 },
    { {1, -SLICE_HEIGHT, 3}, {1, 0, 0}, 0, 0 },
    { {1, -SLICE_HEIGHT, 1}, {1, 0, 0}, 0, 0 },

    /* back of back square */
    { {1, SLICE_HEIGHT, 3}, {0, 0, 1}, 0, 0 },
    { {-1, SLICE_HEIGHT, 3}, {0, 0, 1}, 0, 0 },
    { {-1, -SLICE_HEIGHT, 3}, {0, 0, 1}, 0, 0 },
    { {1, -SLICE_HEIGHT, 3}, {0, 0, 1}, 0, 0 },

    /* left of back square */
    { {-1, SLICE_HEIGHT, 3}, {-1, 0, 0}, 0, 0 },
    { {-1, SLICE_HEIGHT, 1}, {-1, 0, 0}, 0, 0 },
    { {-1, -SLICE_HEIGHT, 1}, {-1, 0, 0}, 0, 0 },
    { {-1, -SLICE_HEIGHT, 3}, {-1, 0, 0}, 0, 0 },

    /* back of left square */
    { {-1, SLICE_HEIGHT, 1}, {0, 0, 1}, 0, 0 },
    { {-3, SLICE_HEIGHT, 1}, {0, 0, 1}, 0, 0 },
    { {-3, -SLICE_HEIGHT, 1}, {0, 0, 1}, 0, 0 },
    { {-1, -SLICE_HEIGHT, 1}, {0, 0, 1}, 0, 0 },

    /* left of left square */
    { {-3, SLICE_HEIGHT, 1}, {-1, 0, 0}, 0, 0 },
    { {-3, SLICE_HEIGHT, -1}, {-1, 0, 0}, 0, 0 },
    { {-3, -SLICE_HEIGHT, -1}, {-1, 0, 0}, 0, 0 },
    { {-3, -SLICE_HEIGHT, 1}, {-1, 0, 0}, 0, 0 },

    /* front of left square */
    { {-3, SLICE_HEIGHT, -1}, {0, 0, -1}, 0, 0 },
    { {-1, SLICE_HEIGHT, -1}, {0, 0, -1}, 0, 0 },
    { {-1, -SLICE_HEIGHT, -1}, {0, 0, -1}, 0, 0 },
    { {-3, -SLICE_HEIGHT, -1}, {0, 0, -1}, 0, 0 },

    /* left of front square */
    { {-1, SLICE_HEIGHT, -1}, {-1, 0, 0}, 0, 0 },
    { {-1, SLICE_HEIGHT, -3}, {-1, 0, 0}, 0, 0 },
    { {-1, -SLICE_HEIGHT, -3}, {-1, 0, 0}, 0, 0 },
    { {-1, -SLICE_HEIGHT, -1}, {-1, 0, 0}, 0, 0 }
};

static vertex_out_attrs cross_transformed_vertices[CROSS_VERTEX_COUNT];
static gfx3d_face cross_faces[CROSS_FACE_COUNT] = {
    {0, 1, 2}, {0, 2, 3},
    {4, 5, 6}, {4, 6, 7},
    {8, 9, 10}, {8, 10, 11},
    {12, 13, 14}, {12, 14, 15},
    {16, 17, 18}, {16, 18, 19},
    {20, 21, 22}, {20, 22, 23},
    {24, 25, 26}, {24, 26, 27},
    {28, 29, 30}, {28, 30, 31},
    {32, 33, 34}, {32, 34, 35},
    {36, 37, 38}, {36, 38, 39},
    {40, 41, 42}, {40, 42, 43},
    {44, 45, 46}, {44, 46, 47},
    {48, 49, 50}, {48, 50, 51},
    {52, 53, 54}, {52, 54, 55},
    {56, 57, 58}, {56, 58, 59},
    {60, 61, 62}, {60, 62, 63},
    {64, 65, 66}, {64, 66, 67},
    {68, 69, 70}, {68, 70, 71},
    {72, 73, 74}, {72, 74, 75},
    {76, 77, 78}, {76, 78, 79},
    {80, 81, 82}, {80, 82, 83},
    {84, 85, 86}, {84, 86, 87}
};



void twister_init(void) {
    square.vertex_count = SQUARE_VERTEX_COUNT;
    square.vertices = square_vertices;
    square.transformed_vertices = square_transformed_vertices;
    square.face_count = SQUARE_FACE_COUNT;
    square.faces = square_faces;

    cross.vertex_count = CROSS_VERTEX_COUNT;
    cross.vertices = cross_vertices;
    cross.transformed_vertices = cross_transformed_vertices;
    cross.face_count = CROSS_FACE_COUNT;
    cross.faces = cross_faces;
}

void twister_frame(uint32_t *pixels, uint32_t time) {
    mat4 rotate_matrix;
    mat3 normal_rotate_matrix;
    // mat4 projection_matrix;

    gfx_cls(pixels, 0x00000000);
    gfx3d_clear_zbuffer(zbuffer);

    mat4_identity(rotate_matrix);
    mat4_rotate_y(rotate_matrix, ((double)time) / 1000);
    mat4_to_inverse_transpose_mat3(rotate_matrix, normal_rotate_matrix);

    vec3 light_pos = {0.1, 4, -2};

    vec3 trans1 = {0, 3, 6};
    mat4_translate(rotate_matrix, trans1);
    gfx3d_gouraud_mesh(pixels, zbuffer, square, rotate_matrix, normal_rotate_matrix, light_pos, 0x00ffff00);

    trans1.y = -0.5;
    trans1.z = 0;

    mat4_translate(rotate_matrix, trans1);
    gfx3d_gouraud_mesh(pixels, zbuffer, square, rotate_matrix, normal_rotate_matrix, light_pos, 0x00ffff00);

    mat4_translate(rotate_matrix, trans1);
    gfx3d_gouraud_mesh(pixels, zbuffer, square, rotate_matrix, normal_rotate_matrix, light_pos, 0x00ffff00);

    mat4_translate(rotate_matrix, trans1);
    gfx3d_gouraud_mesh(pixels, zbuffer, square, rotate_matrix, normal_rotate_matrix, light_pos, 0x00ffff00);

    mat4_translate(rotate_matrix, trans1);
    gfx3d_gouraud_mesh(pixels, zbuffer, cross, rotate_matrix, normal_rotate_matrix, light_pos, 0x00ffff00);

    mat4_translate(rotate_matrix, trans1);
    gfx3d_gouraud_mesh(pixels, zbuffer, cross, rotate_matrix, normal_rotate_matrix, light_pos, 0x00ffff00);

    mat4_translate(rotate_matrix, trans1);
    gfx3d_gouraud_mesh(pixels, zbuffer, cross, rotate_matrix, normal_rotate_matrix, light_pos, 0x00ffff00);

    mat4_translate(rotate_matrix, trans1);
    gfx3d_gouraud_mesh(pixels, zbuffer, cross, rotate_matrix, normal_rotate_matrix, light_pos, 0x00ffff00);

    mat4_translate(rotate_matrix, trans1);
    gfx3d_gouraud_mesh(pixels, zbuffer, square, rotate_matrix, normal_rotate_matrix, light_pos, 0x00ffff00);

    mat4_translate(rotate_matrix, trans1);
    gfx3d_gouraud_mesh(pixels, zbuffer, square, rotate_matrix, normal_rotate_matrix, light_pos, 0x00ffff00);

    mat4_translate(rotate_matrix, trans1);
    gfx3d_gouraud_mesh(pixels, zbuffer, square, rotate_matrix, normal_rotate_matrix, light_pos, 0x00ffff00);
    mat4_translate(rotate_matrix, trans1);
    gfx3d_gouraud_mesh(pixels, zbuffer, square, rotate_matrix, normal_rotate_matrix, light_pos, 0x00ffff00);
}
