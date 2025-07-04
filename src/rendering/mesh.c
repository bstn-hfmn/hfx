#include "rendering/mesh.h"

#include <string.h>
#include <rendering/shader.h>

PMESH HFX_MeshCreate(
    const usize vertices,
    const usize indices)
{
    struct MESH* mesh = malloc(sizeof(struct MESH));
    mesh->verticesCount = vertices;
    mesh->indicesCount = indices;

    mesh->vertices = malloc(sizeof(struct VERTEX) * vertices);
    memset(mesh->vertices, 0, vertices * sizeof(struct VERTEX));

    mesh->indices = malloc(indices * sizeof(u32));
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

    const struct VERTEX_ATTRIBUTE attributes[] = {
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


void HFX_DrawMesh(PMESH mesh, PSHADER shader)
{
    GL_CALL(glUseProgram(shader->program));
    GL_CALL(glBindVertexArray(mesh->vao));

    GL_CALL(glDrawElements(
        GL_TRIANGLES,
        (GLsizei)mesh->indicesCount,
        GL_UNSIGNED_INT,
        nullptr));

    glBindVertexArray(0);
}