#include "rendering/renderer.h"
#include "memory/memory.h"

struct RENDERER g_Renderer;

bool HFX_Init()
{
    HFX_MemoryArenaInit();
    g_Renderer.camera = HFX_ALLOC(sizeof(struct CAMERA));

    glm_vec3_zero(g_Renderer.camera->transform.translation);
    glm_vec3_zero(g_Renderer.camera->transform.rotation);
    glm_vec3_one(g_Renderer.camera->transform.scale);
    glm_perspective(GLM_PI / 2.0f, 16.0f / 9.0f, 0.1f, 100.0f, g_Renderer.camera->projection);

    return true;
}

void HFX_Destroy()
{
    HFX_WindowDestroy(g_Renderer.window);
    HFX_MemoryArenaFree();
}

void HFX_RendererDrawMesh(PMESH mesh, PSHADER shader, struct TRANSFORM transform)
{
    GLint   projectionLocation,
            viewLocation,
            modelLocation;

    GL_CALL(projectionLocation = glGetUniformLocation(shader->program, "hfx_uProjection"));
    GL_CALL(viewLocation = glGetUniformLocation(shader->program, "hfx_uView"));
    GL_CALL(modelLocation = glGetUniformLocation(shader->program, "hfx_uModel"));

    mat4    cameraMatrix = GLM_MAT4_IDENTITY_INIT,
            cameraViewMatrix = GLM_MAT4_IDENTITY_INIT,
            modelMatrix = GLM_MAT4_IDENTITY_INIT;

    HFX_TransformGetMatrix(&transform, modelMatrix);
    HFX_TransformGetMatrix(&g_Renderer.camera->transform, cameraMatrix);

    glm_mat4_inv(cameraMatrix, cameraViewMatrix);

    GL_CALL(glUseProgram(shader->program));
    GL_CALL(glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, (const GLfloat*)g_Renderer.camera->projection));
    GL_CALL(glUniformMatrix4fv(viewLocation, 1, GL_FALSE, (const GLfloat*)cameraViewMatrix));
    GL_CALL(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, (const GLfloat*)modelMatrix));

    GL_CALL(glBindVertexArray(mesh->vao));
    GL_CALL(glDrawElements(
        GL_TRIANGLES,
        (GLsizei)mesh->indicesCount,
        GL_UNSIGNED_INT,
        nullptr));

    glBindVertexArray(0);
}

struct RENDERER* HFX_GetRenderer()
{
    return &g_Renderer;
}
