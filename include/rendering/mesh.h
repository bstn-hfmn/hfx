#ifndef HFX_MESH_H
#define HFX_MESH_H

#include "common.h"
#include "buffer.h"
#include <cglm/cglm.h>
#include "rendering/shader.h"

#define HFX_MESH_VERTICES_SET(dst, src, size)\
    {\
    if (start >= mesh->verticesCount || count > mesh->verticesCount ||\
        start + count > mesh->verticesCount) { return; }\
    for (usize i = start; i < start + count; i++)\
    {\
        memcpy(mesh->vertices[i].dst, src[i], size);\
    }\
    }

enum
{
    HFX_PRIMITIVE_CUBE,
    HFX_PRIMITIVE_PYRAMID,
    HFX_PRIMITIVE_SPHERE
};

struct VERTEX {
  vec3 position;
  vec3 normal;
  vec4 tangent;
  vec4 color;
  vec2 texCoord;
};

struct MESH {
  u32 verticesCount;
  u32 indicesCount;

  u32* indices;
  struct VERTEX* vertices;

  VBO vbo;
  EBO ebo;
  VAO vao;
};

typedef struct MESH *PMESH, MESH_T;
typedef struct VERTEX *PVERTEX, VERTEX_T;

HFX_API void    HFX_DrawMesh(PMESH mesh, PSHADER shader);

HFX_API PMESH   HFX_MeshCreatePrimitive(
    HFX_ENUM primitive);

HFX_API PMESH   HFX_MeshCreate(
    usize vertices,
    usize indices);

HFX_API void    HFX_MeshDestroy(
    PMESH mesh);

HFX_API void    HFX_MeshUploadBuffers(
    PMESH mesh);

HFX_API void    HFX_MeshRecalculateNormals(
    PMESH mesh);

HFX_API void    HFX_MeshRecalculateTangents(
    PMESH mesh);

HFX_API void    HFX_MeshSetVertexPositions(
    PMESH       mesh,
    usize       start,
    usize       count,
    const vec3  positions[]);

HFX_API void    HFX_MeshSetTexCoords(
    PMESH       mesh,
    usize       start,
    usize       count,
    const vec2  texcoords[]);

HFX_API void    HFX_MeshSetVertexColors(
    PMESH       mesh,
    usize       start,
    usize       count,
    const vec4  colors[]);

HFX_API void    HFX_MeshSetIndices(
    PMESH       mesh,
    usize       start,
    usize       count,
    const u32   indices[]);

#endif //HFX_MESH_H
