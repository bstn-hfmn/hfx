#include "rendering/mesh.h"
#include "rendering/shader.h"
#include "memory/memory.h"
#include <string.h>


void HFX_MeshSetNormals(PMESH mesh, const usize start, const usize count, vec3 normals[])
{
    HFX_MESH_VERTICES_SET(normal, normals, sizeof(vec3));
}

PMESH CreatePrimitiveCube()
{
    static vec3 g_CubeVertexPositions[] = {
        {-0.5f, -0.5f,  0.5f},
        { 0.5f, -0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f},

        {-0.5f, -0.5f,  -0.5f},
        { 0.5f, -0.5f,  -0.5f},
        { 0.5f,  0.5f,  -0.5f},
        {-0.5f,  0.5f,  -0.5f},

        { -0.5f, -0.5f, -0.5f },
        { -0.5f, -0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f, -0.5f },

        { 0.5f, -0.5f,  0.5f },
        { 0.5f, -0.5f, -0.5f },
        { 0.5f,  0.5f, -0.5f },
        { 0.5f,  0.5f,  0.5f },

        { -0.5f, 0.5f,  0.5f },
        {  0.5f, 0.5f,  0.5f },
        {  0.5f, 0.5f, -0.5f },
        { -0.5f, 0.5f, -0.5f },

        { -0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f,  0.5f },
        { -0.5f, -0.5f,  0.5f },
    };

    static vec2 g_CubeTexCoords[] = {
        {0.0f, 0.0f},{1.0f, 0.0f},
        {1.0f, 1.0f},{0.0f, 1.0f},

        {1.0f, 0.0f},{0.0f, 0.0f},
        {0.0f, 1.0f},{1.0f, 1.0f},

        {0.0f, 0.0f},{1.0f, 0.0f},
        {1.0f, 1.0f},{0.0f, 1.0f},

        {0.0f, 0.0f},{1.0f, 0.0f},
        {1.0f, 1.0f},{0.0f, 1.0f},

        {0.0f, 0.0f},{1.0f, 0.0f},
        {1.0f, 1.0f},{0.0f, 1.0f},

        {0.0f, 0.0f},{1.0f, 0.0f},
        {1.0f, 1.0f},{0.0f, 1.0f},
    };

    struct MESH* mesh = HFX_MeshCreate(24, 36);
    HFX_MeshSetVertexPositions(mesh, 0, 24, g_CubeVertexPositions);
    HFX_MeshSetTexCoords(mesh, 0, 24, g_CubeTexCoords);
    HFX_MeshSetIndices(mesh, 0, 36, (u32[]){
         0,  1,  2,  2,  3,  0,
         4,  6,  5,  6,  4,  7,
         8,  9,  10, 10, 11, 8,
         12, 13, 14, 14, 15, 12,
         16, 17, 18, 18, 19, 16,
         20, 21, 22, 22, 23, 20
    });

    HFX_MeshRecalculateNormals(mesh, true);
    HFX_MeshUploadBuffers(mesh);

    return mesh;
}

PMESH HFX_MeshPrimitiveCreate(
    HFX_ENUM primitive)
{
    switch (primitive)
    {
    case HFX_PRIMITIVE_CUBE: return CreatePrimitiveCube();

    default: break;
    };

    return nullptr;
}

PMESH HFX_MeshCreate(
    const usize vertices,
    const usize indices)
{
    struct MESH* mesh = HFX_ALLOC(sizeof(struct MESH));
    mesh->verticesCount = vertices;
    mesh->indicesCount = indices;

    mesh->vertices = HFX_ALLOC(sizeof(struct VERTEX) * vertices);
    memset(mesh->vertices, 0, vertices * sizeof(struct VERTEX));

    mesh->indices = HFX_ALLOC(indices * sizeof(u32));
    memset(mesh->indices, 0, indices * sizeof(u32));

    HFX_VertexBufferGenerate(&mesh->vbo);
    HFX_ElementBufferGenerate(&mesh->ebo);
    HFX_VertexArrayGenerate(&mesh->vao);

    return mesh;
}

void HFX_MeshDestroy(
    PMESH mesh)
{
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);
}

void HFX_MeshUploadBuffers(
    PMESH mesh)
{
    HFX_VertexArrayBind(mesh->vao);

    HFX_VertexBufferBind(mesh->vbo);
    HFX_VertexBufferData(
        mesh->vbo,
        mesh->vertices,
        sizeof(struct VERTEX) * mesh->verticesCount,
        HFX_USAGE_STATIC);

    HFX_ElementBufferBind(mesh->ebo);
    HFX_ElementBufferData(mesh->ebo, mesh->indices, mesh->indicesCount * sizeof(u32), HFX_USAGE_STATIC);

    struct VERTEX_ATTRIBUTE attributes[] = {
        { .dataCount = 3, .dataType = HFX_TYPE_FLOAT, .offset = offsetof(struct VERTEX, position), false },
        { .dataCount = 3, .dataType = HFX_TYPE_FLOAT, .offset = offsetof(struct VERTEX, normal), false },
        { .dataCount = 4, .dataType = HFX_TYPE_FLOAT, .offset = offsetof(struct VERTEX, tangent), false },
        { .dataCount = 4, .dataType = HFX_TYPE_FLOAT, .offset = offsetof(struct VERTEX, color), false },
        { .dataCount = 2, .dataType = HFX_TYPE_FLOAT, .offset = offsetof(struct VERTEX, texCoord), false },
    };

    HFX_VertexArrayData(mesh->vao, attributes, sizeof(struct VERTEX), 5);
}

void HFX_MeshRecalculateNormals(
    PMESH mesh,
    bool flat)
{
    if (!mesh || !mesh->indices || !mesh->vertices) return;

    for (int i = 0; i < mesh->verticesCount; i++)
        glm_vec3_zero(mesh->vertices[i].normal);

    for (int i = 0; i < mesh->indicesCount; i += 3)
    {
        const u32 i0 = mesh->indices[i];
        const u32 i1 = mesh->indices[i + 1];
        const u32 i2 = mesh->indices[i + 2];

        struct VERTEX* v0 = &mesh->vertices[i0];
        struct VERTEX* v1 = &mesh->vertices[i1];
        struct VERTEX* v2 = &mesh->vertices[i2];

        vec3 edge0, edge1, normal;
        glm_vec3_sub(v1->position, v0->position, edge0);
        glm_vec3_sub(v2->position, v0->position, edge1);
        glm_vec3_cross(edge0, edge1, normal);
        glm_vec3_normalize(normal);

        if (!flat)
        {
            glm_vec3_add(normal, v0->normal, v0->normal);
            glm_vec3_add(normal, v1->normal, v1->normal);
            glm_vec3_add(normal, v2->normal, v2->normal);
        } else {
            glm_vec3_copy(normal, v0->normal);
            glm_vec3_copy(normal, v1->normal);
            glm_vec3_copy(normal, v2->normal);
        }
    }

    if (flat)
    {
        for (int i = 0; i < mesh->verticesCount; i++)
            glm_vec3_normalize(mesh->vertices[i].normal);
    }
}

void HFX_MeshRecalculateTangents(PMESH mesh)
{
    // TODO: Implement
}

void HFX_MeshSetVertexPositions(
    PMESH mesh,
    const usize start,
    const usize count,
    const vec3 positions[])
{
    HFX_MESH_VERTICES_SET(position, positions, sizeof(vec3));
}

void HFX_MeshSetTexCoords(
    PMESH mesh,
    usize start,
    usize count,
    const vec2 texcoords[])
{
    HFX_MESH_VERTICES_SET(texCoord, texcoords, sizeof(vec2));
}

void HFX_MeshSetVertexColors(
    PMESH mesh,
    usize start,
    usize count,
    const vec4 colors[])
{
    HFX_MESH_VERTICES_SET(color, colors, sizeof(vec4));
}

void HFX_MeshSetIndices(
    PMESH mesh,
    const usize start,
    const usize count,
    const u32 indices[])
{
    if (start >= mesh->indicesCount || count > mesh->indicesCount ||
        start + count > mesh->indicesCount) return;

    memcpy(mesh->indices + start, indices, sizeof(u32) * count);
}