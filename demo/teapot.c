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

struct aiVector3D *transformed_vertices;
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

        transformed_vertices = malloc(vertex_count * sizeof(struct aiVector3D));
        if (transformed_vertices == NULL) {
            printf("could not allocate transformed_vertices\n");
        }
    } else {
        printf("obj import failed: %s\n", aiGetErrorString());
    }
}

void teapot_frame(uint32_t *pixels, uint32_t time) {
    gfx_cls(pixels, 0x00110000);

    double r = ((double)time) / 1000;

    for (unsigned int i = 0; i < vertex_count; i++) {
        struct aiVector3D v = teapot->mVertices[i];

        transformed_vertices[i].x = v.x * cos(r) + v.z * sin(r);
        transformed_vertices[i].y = v.y;
    }

    for (unsigned int i = 0; i < face_count; i++) {
        struct aiFace face = teapot->mFaces[i];
        struct aiVector3D v0 = transformed_vertices[face.mIndices[0]];
        struct aiVector3D v1 = transformed_vertices[face.mIndices[1]];
        struct aiVector3D v2 = transformed_vertices[face.mIndices[2]];

        /* find z component of cross product to test for back-face culling */
        double xpz = (v1.x - v0.x) * (v2.y - v0.y) - (v1.y - v0.y) * (v2.x - v0.x);

        if (xpz < 0) {
            vec3 sv1 = {v0.x / 5, 0.3 - v0.y / 5, v0.z};
            vec3 sv2 = {v1.x / 5, 0.3 - v1.y / 5, v1.z};
            vec3 sv3 = {v2.x / 5, 0.3 - v2.y / 5, v2.z};
            gfx3d_flat_tri(
                pixels, zbuffer, sv1, sv2, sv3,
                (128 + (i & 127)) * 0x01010100
            );
        }
    }
}
