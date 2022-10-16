#include <stdint.h>
#include <assimp/scene.h>
#include "gfx.h"
#include "matrix.h"

typedef struct vertex_in_attrs {
    vec3 position;
    vec3 normal;
    int u;
    int v;
} vertex_in_attrs;

typedef struct vertex_out_attrs {
    vec3 position;
    double brightness;
    int u;
    int v;
} vertex_out_attrs;

typedef struct gfx3d_face {
    unsigned int index1;
    unsigned int index2;
    unsigned int index3;
} gfx3d_face;

typedef struct gfx3d_model {
    unsigned int vertex_count;
    vertex_in_attrs *vertices;
    vertex_out_attrs *transformed_vertices;

    unsigned int face_count;
    gfx3d_face *faces;

    gfx_image texture;
} gfx3d_model;

void gfx3d_load_model(char *model_filename, char *texture_filename, gfx3d_model *model);
void gfx3d_read_mesh(const struct aiScene *scene, int mesh_index, gfx3d_model *model);
void gfx3d_flat_tri(uint32_t *pixels, double *zbuffer, vec3 v1, vec3 v2, vec3 v3, uint32_t colour);
void gfx3d_gouraud_tri(uint32_t *pixels, double *zbuffer, vertex_out_attrs va1, vertex_out_attrs va2, vertex_out_attrs va3, uint32_t colour);
void gfx3d_gouraud_tex_tri(uint32_t *pixels, double *zbuffer, gfx_image *texture, vertex_out_attrs va1, vertex_out_attrs va2, vertex_out_attrs va3);
void gfx3d_clear_zbuffer(double *zbuffer);
void gfx3d_transform_vertices(gfx3d_model mesh, mat4 rotate_matrix, mat3 normal_rotate_matrix, vec3 light_pos);
void gfx3d_gouraud_mesh(uint32_t *pixels, double *zbuffer, gfx3d_model mesh, mat4 rotate_matrix, mat3 normal_rotate_matrix, vec3 light_pos, uint32_t colour);
void gfx3d_gouraud_tex_mesh(uint32_t *pixels, double *zbuffer, gfx3d_model mesh, mat4 rotate_matrix, mat3 normal_rotate_matrix, vec3 light_pos);
