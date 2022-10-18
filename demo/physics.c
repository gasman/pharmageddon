#include <stdlib.h>
#include <math.h>
#include "gfx3d.h"
#include "physics.h"

double zbuffer[192*192];

gfx3d_model box;

#define BOX_VERTEX_COUNT 60
#define BOX_FACE_COUNT 30

vertex_in_attrs box_vertices[BOX_VERTEX_COUNT] = {
    /* position, normal, u, v */
    /* bottom cell base */
    { {-0.333333, -1, 1}, {0, 1, 0}, 0, 0 },
    { {-0.333333, -1, 1.333333}, {0, 1, 0}, 0, 0 },
    { {0.333333, -1, 1.333333}, {0, 1, 0}, 0, 0 },
    { {0.333333, -1, 1}, {0, 1, 0}, 0, 0 },

    /* bottom cell left */
    { {-0.333333, -1, 1}, {1, 0, 0}, 0, 0 },
    { {-0.333333, -0.333333, 1}, {1, 0, 0}, 0, 0 },
    { {-0.333333, -0.333333, 1.333333}, {1, 0, 0}, 0, 0 },
    { {-0.333333, -1, 1.333333}, {1, 0, 0}, 0, 0 },

    /* bottom cell right */
    { {0.333333, -1, 1.333333}, {-1, 0, 0}, 0, 0 },
    { {0.333333, -0.333333, 1.333333}, {-1, 0, 0}, 0, 0 },
    { {0.333333, -0.333333, 1}, {-1, 0, 0}, 0, 0 },
    { {0.333333, -1, 1}, {-1, 0, 0}, 0, 0 },

    /* top cell left */
    { {-0.333333, 0.333333, 1}, {1, 0, 0}, 0, 0 },
    { {-0.333333, 1, 1}, {1, 0, 0}, 0, 0 },
    { {-0.333333, 1, 1.333333}, {1, 0, 0}, 0, 0 },
    { {-0.333333, 0.333333, 1.333333}, {1, 0, 0}, 0, 0 },

    /* top cell right */
    { {0.333333, 0.333333, 1.333333}, {-1, 0, 0}, 0, 0 },
    { {0.333333, 1, 1.333333}, {-1, 0, 0}, 0, 0 },
    { {0.333333, 1, 1}, {-1, 0, 0}, 0, 0 },
    { {0.333333, 0.333333, 1}, {-1, 0, 0}, 0, 0 },

    /* top cell top */
    { {-0.333333, 1, 1.333333}, {0, -1, 0}, 0, 0 },
    { {-0.333333, 1, 1}, {0, -1, 0}, 0, 0 },
    { {0.333333, 1, 1}, {0, -1, 0}, 0, 0 },
    { {0.333333, 1, 1.333333}, {0, -1, 0}, 0, 0 },

    /* left cell base */
    { {-1, -0.333333, 1}, {0, 1, 0}, 0, 0 },
    { {-1, -0.333333, 1.333333}, {0, 1, 0}, 0, 0 },
    { {-0.333333, -0.333333, 1.333333}, {0, 1, 0}, 0, 0 },
    { {-0.333333, -0.333333, 1}, {0, 1, 0}, 0, 0 },

    /* left cell left */
    { {-1, -0.333333, 1}, {1, 0, 0}, 0, 0 },
    { {-1, 0.333333, 1}, {1, 0, 0}, 0, 0 },
    { {-1, 0.333333, 1.333333}, {1, 0, 0}, 0, 0 },
    { {-1, -0.333333, 1.333333}, {1, 0, 0}, 0, 0 },

    /* left cell top */
    { {-1, 0.333333, 1.333333}, {0, -1, 0}, 0, 0 },
    { {-1, 0.333333, 1}, {0, -1, 0}, 0, 0 },
    { {-0.333333, 0.333333, 1}, {0, -1, 0}, 0, 0 },
    { {-0.333333, 0.333333, 1.333333}, {0, -1, 0}, 0, 0 },

    /* right cell base */
    { {0.333333, -0.333333, 1}, {0, 1, 0}, 0, 0 },
    { {0.333333, -0.333333, 1.333333}, {0, 1, 0}, 0, 0 },
    { {1, -0.333333, 1.333333}, {0, 1, 0}, 0, 0 },
    { {1, -0.333333, 1}, {0, 1, 0}, 0, 0 },

    /* right cell right */
    { {1, -0.333333, 1.333333}, {-1, 0, 0}, 0, 0 },
    { {1, 0.333333, 1.333333}, {-1, 0, 0}, 0, 0 },
    { {1, 0.333333, 1}, {-1, 0, 0}, 0, 0 },
    { {1, -0.333333, 1}, {-1, 0, 0}, 0, 0 },

    /* right cell top */
    { {0.333333, 0.333333, 1.333333}, {0, -1, 0}, 0, 0 },
    { {0.333333, 0.333333, 1}, {0, -1, 0}, 0, 0 },
    { {1, 0.333333, 1}, {0, -1, 0}, 0, 0 },
    { {1, 0.333333, 1.333333}, {0, -1, 0}, 0, 0 },

    /* centre back */
    { {-0.333333, -1, 1.333333}, {0, 0, -1}, 0, 0 },
    { {-0.333333, 1, 1.333333}, {0, 0, -1}, 0, 0 },
    { {0.333333, 1, 1.333333}, {0, 0, -1}, 0, 0 },
    { {0.333333, -1, 1.333333}, {0, 0, -1}, 0, 0 },

    /* left back */
    { {-1, -0.333333, 1.333333}, {0, 0, -1}, 0, 0 },
    { {-1, 0.333333, 1.333333}, {0, 0, -1}, 0, 0 },
    { {-0.333333, 0.333333, 1.333333}, {0, 0, -1}, 0, 0 },
    { {-0.333333, -0.333333, 1.333333}, {0, 0, -1}, 0, 0 },

    /* right back */
    { {0.333333, -0.333333, 1.333333}, {0, 0, -1}, 0, 0 },
    { {0.333333, 0.333333, 1.333333}, {0, 0, -1}, 0, 0 },
    { {1, 0.333333, 1.333333}, {0, 0, -1}, 0, 0 },
    { {1, -0.333333, 1.333333}, {0, 0, -1}, 0, 0 }
};
vertex_out_attrs box_transformed_vertices[BOX_VERTEX_COUNT];
gfx3d_face box_faces[BOX_FACE_COUNT] = {
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
    {56, 57, 58}, {56, 58, 59}
};

gfx3d_model sphere;
#define SPHERE_LAT_STEP 10
#define SPHERE_LNG_STEP 10

/* the semicircle from north pole to south pole is divided into SPHERE_LAT_STEP segments,
which by the principle of fenceposts equals SPHERE_LAT_STEP+1 rings of vertices,
each of which consists of SPHERE_LNG_STEP vertices and SPHERE_LNG_STEP edges,
except for the north and south poles which consist of one vertex each.
In other words, there are (SPHERE_LAT_STEP - 1) true rings of SPHERE_LNG_STEP vertices
each, plus two vertices for the poles.
*/
#define SPHERE_VERTEX_COUNT ((SPHERE_LAT_STEP - 1) * SPHERE_LNG_STEP + 2)
/* Each ring of faces consists of SPHERE_LNG_STEP * 2 faces, except for the polar caps
which consist of SPHERE_LNG_STEP */
#define SPHERE_FACE_COUNT (2 * (SPHERE_LAT_STEP - 1) * SPHERE_LNG_STEP)

vertex_in_attrs sphere_vertices[SPHERE_VERTEX_COUNT];
vertex_out_attrs sphere_transformed_vertices[SPHERE_VERTEX_COUNT];
gfx3d_face sphere_faces[SPHERE_FACE_COUNT];

void physics_init(void) {
    box.vertex_count = BOX_VERTEX_COUNT;
    box.vertices = box_vertices;
    box.transformed_vertices = box_transformed_vertices;
    box.face_count = BOX_FACE_COUNT;
    box.faces = box_faces;

    double r = 0.2; // sphere radius
    sphere.vertex_count = SPHERE_VERTEX_COUNT;
    sphere.vertices = sphere_vertices;
    sphere.transformed_vertices = sphere_transformed_vertices;
    sphere.face_count = SPHERE_FACE_COUNT;
    sphere.faces = sphere_faces;
    int v = 0;

    /* north pole */
    sphere.vertices[v].position.x = 0;
    sphere.vertices[v].position.y = r;
    sphere.vertices[v].position.z = 0;
    sphere.vertices[v].normal.x = 0;
    sphere.vertices[v].normal.y = 1;
    sphere.vertices[v].normal.z = 0;
    v++;

    for (int sy = 1; sy < SPHERE_LAT_STEP - 1; sy++) {
        /* vertex ring */
        double lat = M_PI * ((double)sy) / SPHERE_LAT_STEP;
        double c_lat = cos(lat);
        double s_lat = sin(lat);
        double y = r * c_lat;
        double ring_r = r * sin(lat);
        for (int sx = 0; sx < SPHERE_LNG_STEP; sx++) {
            double lng = M_PI * 2 * ((double)sx) / SPHERE_LNG_STEP;
            double s_lng = sin(lng);
            double c_lng = cos(lng);
            sphere.vertices[v].position.x = ring_r * s_lng;
            sphere.vertices[v].position.y = y;
            sphere.vertices[v].position.z = ring_r * c_lng;
            sphere.vertices[v].normal.x = c_lat * s_lng;
            sphere.vertices[v].normal.y = s_lat;
            sphere.vertices[v].normal.z = c_lat * c_lng;
            v++;
        }
    }
    /* south pole */
    sphere.vertices[v].position.x = 0;
    sphere.vertices[v].position.y = -r;
    sphere.vertices[v].position.z = 0;
    sphere.vertices[v].normal.x = 0;
    sphere.vertices[v].normal.y = -1;
    sphere.vertices[v].normal.z = 0;

    int f = 0;
    /* north polar cap */
    for (int sx = 0; sx < SPHERE_LNG_STEP; sx++) {
        sphere.faces[f].index1 = 0;
        sphere.faces[f].index2 = sx + 1;
        sphere.faces[f].index3 = (sx + 1) % SPHERE_LNG_STEP + 1;
        f++;
    }
    for (int sy = 1; sy < SPHERE_LAT_STEP - 2; sy++) {
        /* face ring */
        int v0 = 1 + (sy - 1) * SPHERE_LNG_STEP;  // first index of upper vertex ring
        int v1 = 1 + sy * SPHERE_LNG_STEP;  // first index of lower vertex ring
        for (int sx = 0; sx < SPHERE_LNG_STEP; sx++) {
            sphere.faces[f].index1 = v0 + sx;
            sphere.faces[f].index2 = v1 + sx;
            sphere.faces[f].index3 = v1 + ((sx + 1) % SPHERE_LNG_STEP);
            f++;
            sphere.faces[f].index1 = v0 + sx;
            sphere.faces[f].index2 = v1 + ((sx + 1) % SPHERE_LNG_STEP);
            sphere.faces[f].index3 = v0 + ((sx + 1) % SPHERE_LNG_STEP);
            f++;
        }
    }

    /* south polar cap */
    int v0 = 1 + (SPHERE_LAT_STEP - 2) * SPHERE_LNG_STEP;
    int v1 = 1 + (SPHERE_LAT_STEP - 1) * SPHERE_LNG_STEP;
    for (int sx = 0; sx < SPHERE_LNG_STEP; sx++) {
        sphere.faces[f].index1 = v0 + sx;
        sphere.faces[f].index2 = v1;
        sphere.faces[f].index3 = v0 + (sx + 1) % SPHERE_LNG_STEP;
        f++;
    }
}

void physics_frame(uint32_t *pixels, uint32_t time) {
    mat4 rotate_matrix;
    mat3 normal_rotate_matrix;
    // mat4 projection_matrix;

    gfx_cls(pixels, 0x00000000);
    gfx3d_clear_zbuffer(zbuffer);

    mat4_identity(rotate_matrix);
    mat4_to_inverse_transpose_mat3(rotate_matrix, normal_rotate_matrix);

    vec3 light_pos = {0.1, 2, -2};

    gfx3d_gouraud_mesh(pixels, zbuffer, box, rotate_matrix, normal_rotate_matrix, light_pos, 0x00ffff00);

    // mat4_rotate_x(rotate_matrix, ((double)time) / 900);
    vec3 trans1 = {0, 0, -1.3};
    mat4_translate(rotate_matrix, trans1);
    mat4_to_inverse_transpose_mat3(rotate_matrix, normal_rotate_matrix);
    gfx3d_gouraud_mesh(pixels, zbuffer, sphere, rotate_matrix, normal_rotate_matrix, light_pos, 0xffffff00);
}
