#include "rendering/buffer.h"
#include "memory/memory.h"
#include <string.h>

void SetBufferData(
    const GLenum target,
    const u32 buffer,
    const void* const data,
    const usize size,
    const HFX_ENUM usage)
{
    GL_CALL(glBufferData(target, (GLsizeiptr)size, data, usage));
}

void HFX_ElementBufferGenerate(
    EBO *const ebo)
{
    GL_CALL(glGenBuffers(1, ebo));
}

void HFX_ElementBufferBind(
    const EBO ebo)
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
}


void HFX_ElementBufferData(
    const EBO ebo,
    const void* const data,
    const usize size,
    const HFX_ENUM usage)
{
    SetBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo, data, size, usage);
}

void HFX_VertexBufferGenerate(
    VBO* vbo)
{
    GL_CALL(glGenBuffers(1, vbo));
}

void HFX_VertexBufferBind(
    const VBO vbo)
{
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
}


void HFX_VertexBufferData(
    const VBO vbo,
    const void* const data,
    const usize size,
    const HFX_ENUM usage)
{
    SetBufferData(GL_ARRAY_BUFFER, vbo, data, size, usage);
}

void HFX_VertexArrayGenerate(
    VAO *const vao)
{
    GL_CALL(glGenVertexArrays(1, vao));
}

HFX_API void HFX_VertexArrayBind(
    const VAO vao)
{
    GL_CALL(glBindVertexArray(vao));
}

void HFX_VertexArrayData(
    const VAO vao,
    struct VERTEX_ATTRIBUTE* attributes,
    const usize stride,
    const usize count)
{
    for (usize i = 0; i < count; i++) {
        GL_CALL(glVertexAttribPointer(i,
                              attributes[i].dataCount,
                              attributes[i].dataType,
                              attributes[i].normalized,
                              stride,
                              (GLvoid*)attributes[i].offset));

        GL_CALL(glEnableVertexAttribArray(i));
    }
}

struct FRAMEBUFFER_TARGET HFX_FramebufferTargetTexture2D(
    const HFX_ENUM attachment,
    const HFX_ENUM internal_format,
    const HFX_ENUM texture_format,
    const HFX_ENUM data_type)
{
    return (struct FRAMEBUFFER_TARGET) {
        .targetType = HFX_FRAMEBUFFER_TARGET_TEXTURE_2D,
        .attachment = attachment,
        .internalFormat = internal_format,
        .dataType = data_type,
        .textureFormat = texture_format,
        .targetHandle = HFX_NONE
    };
}

struct FRAMEBUFFER_TARGET HFX_FramebufferTargetRenderbuffer(
    const HFX_ENUM attachment,
    const HFX_ENUM internal_format)
{
    return (struct FRAMEBUFFER_TARGET) {
        .targetType = HFX_FRAMEBUFFER_TARGET_RENDERBUFFER,
        .attachment = attachment,
        .internalFormat = internal_format,
        .dataType = HFX_NONE,
        .textureFormat = HFX_NONE,
        .targetHandle = HFX_NONE
    };
}

PFRAMEBUFFER HFX_FramebufferCreate(
    const usize width,
    const usize height,
    struct FRAMEBUFFER_TARGET* targets,
    const usize count)
{
    if (count > FRAMEBUFFER_TARGET_CAPACITY)
    {
        HFX_LOG(LOG_ERROR, "Framebuffer target count exceeds maximum count, "
                           "there can't be more than %d targets.", FRAMEBUFFER_TARGET_CAPACITY);

        HFX_SetLastError("Framebuffer target count exceed maximum count");
        return nullptr;
    }

    struct FRAMEBUFFER framebuffer;

    framebuffer.width = width;
    framebuffer.height = height;
    framebuffer.targetCount = 0;

    memset(framebuffer.targets, 0, sizeof(struct FRAMEBUFFER_TARGET) * count);
    memcpy(framebuffer.targets, targets, sizeof(struct FRAMEBUFFER_TARGET) * count);

    GL_CALL(glGenFramebuffers(1, &framebuffer.handle));
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.handle));

    for (usize i = 0; i < count; i++)
    {
        struct FRAMEBUFFER_TARGET* target = &framebuffer.targets[i];
        switch (target->targetType)
        {
        case HFX_FRAMEBUFFER_TARGET_TEXTURE_2D:
            {
                GL_CALL(glGenTextures(1, (GLuint*)&target->targetHandle));
                GL_CALL(glBindTexture(GL_TEXTURE_2D, target->targetHandle));
                GL_CALL(glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    target->internalFormat,
                    (GLint)framebuffer.width,
                    (GLint)framebuffer.height,
                    0,
                    target->textureFormat,
                    target->dataType,
                    nullptr));

                GL_CALL(glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    target->attachment,
                    GL_TEXTURE_2D,
                    target->targetHandle,
                    0));
            } break;

        case HFX_FRAMEBUFFER_TARGET_RENDERBUFFER:
            {
                GL_CALL(glGenRenderbuffers(1, (GLuint*)&target->targetHandle));
                GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, target->targetHandle));

                GL_CALL(glRenderbufferStorage(
                    GL_RENDERBUFFER,
                    target->internalFormat,
                    (GLint)framebuffer.width,
                    (GLint)framebuffer.height));

                GL_CALL(glFramebufferRenderbuffer(
                    GL_FRAMEBUFFER,
                    target->attachment,
                    GL_RENDERBUFFER,
                    target->targetHandle));
            } break;

        default:
            {
                HFX_LOG(LOG_ERROR, "Tried to generate framebuffer for unknown target");
                HFX_SetLastError("Tried to generate framebuffer for unknown target");

                HFX_FramebufferDestroy(&framebuffer);
                GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
            } return nullptr;
        }

        framebuffer.targetCount++;
    }

    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        HFX_LOG(LOG_ERROR, "Framebuffer is not complete");
        HFX_SetLastError("Framebuffer is not complete");

        HFX_FramebufferDestroy(&framebuffer);
        return nullptr;
    }

    struct FRAMEBUFFER* fb = HFX_ALLOC(sizeof(struct FRAMEBUFFER));
    memcpy(fb, &framebuffer, sizeof(struct FRAMEBUFFER));
    return fb;
}

void HFX_FramebufferBind(
    PFRAMEBUFFER framebuffer)
{
    GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->handle));
    GL_CALL(glViewport(0, 0, (GLint)framebuffer->width, (GLint)framebuffer->height));
}

u32 HFX_FramebufferGetTarget(
    PFRAMEBUFFER framebuffer,
    const HFX_ENUM attachment)
{
    for (usize i = 0; i < framebuffer->targetCount; i++)
    {
        const struct FRAMEBUFFER_TARGET target = framebuffer->targets[i];
        if (target.attachment == attachment)
            return target.targetHandle;
    }

    return HFX_NONE;
}

void HFX_FramebufferDestroy(
    PFRAMEBUFFER buffer)
{
    if (buffer->handle != GL_NONE)
        GL_CALL(glDeleteFramebuffers(1, &buffer->handle));

    for (usize i = 0; i < buffer->targetCount; i++)
    {
        const struct FRAMEBUFFER_TARGET target = buffer->targets[i];
        switch (target.targetType)
        {
        case HFX_FRAMEBUFFER_TARGET_TEXTURE_2D: GL_CALL(glDeleteTextures(1, &target.targetHandle)); break;
        case HFX_FRAMEBUFFER_TARGET_RENDERBUFFER: GL_CALL(glDeleteRenderbuffers(1, &target.targetHandle)); break;
        default: break;
        }
    }
}

void HFX_FramebufferResize(
    PFRAMEBUFFER* buffer,
    const usize width,
    const usize height)
{
    if (buffer == nullptr || (buffer != nullptr && (*buffer) == nullptr))
        return;

    const u32 count = (*buffer)->targetCount;
    struct FRAMEBUFFER_TARGET targets[FRAMEBUFFER_TARGET_CAPACITY];
    memcpy(targets, (*buffer)->targets, count * sizeof(struct FRAMEBUFFER_TARGET));

    if ((*buffer) != nullptr)
    {
        HFX_FramebufferDestroy(*buffer);
    }

    *buffer = HFX_FramebufferCreate(width, height, targets, count);
}