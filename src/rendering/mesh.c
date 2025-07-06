#include "rendering/mesh.h"
#include "rendering/shader.h"
#include "memory/memory.h"
#include <string.h>


PMESH CreatePrimitiveCube()
{
    static vec3 g_CubeVertexPositions[] = {
        {-0.5f, -0.5f, -0.5f}, // 0: Left  Bottom Back
        { 0.5f, -0.5f, -0.5f}, // 1: Right Bottom Back
        { 0.5f,  0.5f, -0.5f}, // 2: Right Top    Back
        {-0.5f,  0.5f, -0.5f}, // 3: Left  Top    Back
        {-0.5f, -0.5f,  0.5f}, // 4: Left  Bottom Front
        { 0.5f, -0.5f,  0.5f}, // 5: Right Bottom Front
        { 0.5f,  0.5f,  0.5f}, // 6: Right Top    Front
        {-0.5f,  0.5f,  0.5f}, // 7: Left  Top    Front
    };


    struct MESH* mesh = HFX_MeshCreate(8, 36);
    HFX_MeshSetVertexPositions(mesh, 0, 8, g_CubeVertexPositions);
    HFX_MeshSetIndices(mesh, 0, 36, (u32[]){
        // Back face
       0, 1, 2,
       2, 3, 0,

       // Front face
       4, 6, 5,
       6, 4, 7,

       // Left face
       0, 3, 7,
       7, 4, 0,

       // Right face
       1, 5, 6,
       6, 2, 1,

       // Bottom face
       0, 4, 5,
       5, 1, 0,

       // Top face
       3, 2, 6,
       6, 7, 3
    });
    HFX_MeshUploadBuffers(mesh);

    return mesh;
}

PMESH HFX_MeshCreatePrimitive(
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
    HFX_FREE(mesh->vertices);
    HFX_FREE(mesh->indices);

    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);

    HFX_FREE(mesh);
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

void HFX_MeshRecalculateNormals(PMESH mesh)
{
}

void HFX_MeshRecalculateTangents(PMESH mesh)
{
}

void HFX_MeshSetVertexPositions(
    PMESH mesh,
    const usize start,
    const usize count,
    const vec3 positions[])
{
    if (start >= mesh->verticesCount || count > mesh->verticesCount ||
        start + count > mesh->verticesCount) return;

    for (usize i = start; i < start + count; i++)
    {
        memcpy(mesh->vertices[i].position, positions[i], sizeof(vec3));
    }
}

void HFX_MeshSetTexCoords(PMESH mesh, usize start, usize count, const vec2 texcoords[])
{
}

void HFX_MeshSetVertexColors(PMESH mesh, const vec4 colors[])
{
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