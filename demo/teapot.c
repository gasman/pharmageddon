#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags
#include "gfx.h"
#include "gfx3d.h"
#include "teapot.h"

vec3 *model_vertices, *transformed_vertices;
const struct aiMesh *teapot;
unsigned int vertex_count;
unsigned int face_count;

double zbuffer[192*192];

void teapot_init(void) {
    // Start the import on the given file with some example postprocessing
    // Usually - if speed is not the most important aspect for you - you'll t
    // probably to request more postprocessing than we do in this example.
    const struct aiScene* scene = aiImportFile( "../assets/teapot.obj",
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);

    if (scene != NULL) {
        teapot = scene->mMeshes[0];
        vertex_count = teapot->mNumVertices;
        face_count = teapot->mNumFaces;

        model_vertices = malloc(vertex_count * sizeof(vec3));
        if (model_vertices == NULL) {
            printf("could not allocate model_vertices\n");
        }

        for (unsigned int i = 0; i < vertex_count; i++) {
            struct aiVector3D v = teapot->mVertices[i];
            model_vertices[i].x = v.x;
            model_vertices[i].y = v.y;
            model_vertices[i].z = v.z;
        }

        transformed_vertices = malloc(vertex_count * sizeof(vec3));
        if (transformed_vertices == NULL) {
            printf("could not allocate transformed_vertices\n");
        }
    } else {
        printf("obj import failed: %s\n", aiGetErrorString());
    }
}

void teapot_frame(uint32_t *pixels, uint32_t time) {
    mat4 rotate_matrix;
    // mat4 projection_matrix;

    gfx_cls(pixels, 0x00110000);

    mat4_identity(rotate_matrix);
    vec3 trans1 = {0, -1, 1};
    mat4_translate(rotate_matrix, trans1);
    mat4_rotate_y(rotate_matrix, ((double)time) / 1000);
    mat4_rotate_x(rotate_matrix, ((double)time) / 900);
    // mat4_ortho( projection_matrix, -5.0, 5.0, 5.0, -5.0, -30.0, 100.0 );

    for (unsigned int i = 0; i < vertex_count; i++) {
        vec3 pos = mat4_mul_vec3(model_vertices[i], rotate_matrix);
        transformed_vertices[i] = pos; // mat4_mul_vec3(pos, projection_matrix);
    }

    for (double *zbuffer_ptr = zbuffer; zbuffer_ptr < (zbuffer + 192*192); zbuffer_ptr++) {
        *zbuffer_ptr = 100000;
    }

    for (unsigned int i = 0; i < face_count; i++) {
        struct aiFace face = teapot->mFaces[i];
        vec3 v0 = transformed_vertices[face.mIndices[0]];
        vec3 v1 = transformed_vertices[face.mIndices[1]];
        vec3 v2 = transformed_vertices[face.mIndices[2]];

        vec3 sv1 = {v0.x/5, v0.y/5, v0.z};
        vec3 sv2 = {v1.x/5, v1.y/5, v1.z};
        vec3 sv3 = {v2.x/5, v2.y/5, v2.z};
        gfx3d_flat_tri(
            pixels, zbuffer, sv1, sv2, sv3,
            (128 + (i & 127)) * 0x01010100
        );
    }
}
