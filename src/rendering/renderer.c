#include "rendering/renderer.h"
#include "memory/memory.h"

struct RENDERER g_Renderer;

bool HFX_Init(
    const char* title,
    usize width,
    usize height)
{
    HFX_MemoryArenaInit();
    g_Renderer.camera = HFX_ALLOC(sizeof(struct CAMERA));
    g_Renderer.window = HFX_WindowCreate(title, width, height);

    if (g_Renderer.window == NULL)
    {
        HFX_LOG(LOG_ERROR, "Failed to create window (Error: %s)\n", HFX_GetLastError());
        HFX_SetLastError("Failed to initialize window");
        return false;
    }

    usize framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(g_Renderer.window->handle, (int*)&framebufferWidth, (int*)&framebufferHeight);
    const f32 aspectRatio = (f32)framebufferWidth / (f32)framebufferHeight;

    glm_perspective(glm_rad(45.0f), aspectRatio, 0.1f, 100.0f, g_Renderer.camera->projection);

    glm_vec3_zero(g_Renderer.camera->transform.translation);
    glm_vec3_zero(g_Renderer.camera->transform.rotation);
    glm_vec3_one(g_Renderer.camera->transform.scale);

    glEnable(GL_CULL_FACE);
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
    struct TRANSFORM transform,
    PTEXTURE texture)
{
    if (texture != nullptr)
        glBindTexture(GL_TEXTURE_2D, texture->handle);

    GLint   projectionLocation,
            viewLocation,
            modelLocation;

    GL_CALL(projectionLocation = glGetUniformLocation(shader->program, "hfx_uProjection"));
    GL_CALL(viewLocation = glGetUniformLocation(shader->program, "hfx_uView"));
    GL_CALL(modelLocation = glGetUniformLocation(shader->program, "hfx_uModel"));

    mat4    cameraViewMatrix = GLM_MAT4_IDENTITY_INIT,
            modelMatrix = GLM_MAT4_IDENTITY_INIT;

    HFX_TransformGetToWorldMatrix(&transform, modelMatrix);
    HFX_TransformGetToLocalMatrix(&g_Renderer.camera->transform, cameraViewMatrix);

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

struct CAMERA* HFX_GetCamera()
{
    return g_Renderer.camera;
}

struct WINDOW* HFX_GetWindow()
{
    return g_Renderer.window;
}

struct RENDERER* HFX_GetRenderer()
{
    return &g_Renderer;
}
