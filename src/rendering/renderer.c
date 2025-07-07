#include "rendering/renderer.h"
#include "memory/memory.h"

struct RENDERER g_Renderer;

bool HFX_Init()
{
    HFX_MemoryArenaInit();
    g_Renderer.camera = HFX_ALLOC(sizeof(struct CAMERA));

    glm_vec3_zero(g_Renderer.camera->transform.translation);
    glm_vec3_make((vec3){ 0, 0, 5.0f }, g_Renderer.camera->transform.translation);

    glm_vec3_zero(g_Renderer.camera->transform.rotation);
    glm_vec3_one(g_Renderer.camera->transform.scale);

    return true;
}

void HFX_Destroy()
{
    HFX_WindowDestroy(g_Renderer.window);
    HFX_MemoryArenaFree();
}

void HFX_RendererDrawMesh(
    PMESH mesh,
    PSHADER shader,
    struct TRANSFORM transform)
{

    int actualWidth, actualHeight;
    glfwGetFramebufferSize(g_Renderer.window->handle, &actualWidth, &actualHeight);
    float aspectRatio = (float)actualWidth / (float)actualHeight;

    glm_perspective(glm_rad(45.0f), aspectRatio, 0.1f, 100.0f, g_Renderer.camera->projection);

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

void HFX_RendererDrawMeshEx(
    PMESH mesh,
    PSHADER shader,
    struct TRANSFORM transform,
    vec4 color,
    PTEXTURE texture)
{
    glBindTexture(GL_TEXTURE_2D, texture->handle);
    HFX_RendererDrawMesh(mesh, shader, transform);
}

struct RENDERER* HFX_GetRenderer()
{
    return &g_Renderer;
}
