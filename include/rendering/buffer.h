#ifndef HFX_BUFFER_H
#define HFX_BUFFER_H

#include "common.h"
#include <glad/glad.h>

#define FRAMEBUFFER_TARGET_CAPACITY 8

struct FRAMEBUFFER_TARGET
{
  HFX_ENUM textureFormat;
  HFX_ENUM internalFormat;
  HFX_ENUM dataType;

  GL_HANDLE targetHandle;
  HFX_ENUM targetType;
  HFX_ENUM attachment;
};


struct FRAMEBUFFER
{
  usize width;
  usize height;

  GL_HANDLE handle;
  usize targetCount;
  struct FRAMEBUFFER_TARGET targets[FRAMEBUFFER_TARGET_CAPACITY];
};

struct VERTEX_ATTRIBUTE
{
  u32 dataCount;
  HFX_ENUM dataType;
  u32 offset;
  bool normalized;
};

typedef GL_HANDLE EBO;
typedef GL_HANDLE VBO;
typedef GL_HANDLE VAO;

typedef struct FRAMEBUFFER *PFRAMEBUFFER, FRAMEBUFFER_T;
typedef struct FRAMEBUFFER_TARGET *PFRAMEBUFFER_TARGET, FRAMEBUFFER_TARGET_T;
typedef struct VERTEX_ATTRIBUTE *PVERTEX_ATTRIBUTE, VERTEX_ATTRIBUTE_T;

enum
{
  HFX_FRAMEBUFFER_TARGET_TEXTURE_2D,
  HFX_FRAMEBUFFER_TARGET_RENDERBUFFER
};

enum
{
  HFX_ATTACHMENT_COLOR0 = GL_COLOR_ATTACHMENT0,
  HFX_ATTACHMENT_DEPTH = GL_DEPTH_ATTACHMENT,
  HFX_ATTACHMENT_STENCIL = GL_STENCIL_ATTACHMENT,
  HFX_ATTACHMENT_DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT,
};

enum
{
  HFX_USAGE_DYNAMIC = GL_DYNAMIC_DRAW,
  HFX_USAGE_STATIC = GL_STATIC_DRAW,
  HFX_USAGE_STREAM = GL_STREAM_DRAW,
};


/**
 * Creates and initializes a framebuffer object with the specified width, height,
 * and a set of framebuffer targets (textures or renderbuffer objects). Validates the
 * number of targets and sets up appropriate OpenGL storage and attachments.
 *
 * @param width The width of the framebuffer (in pixels)
 * @param height The height of the framebuffer (in pixels)
 * @param targets The targets (either a Texture2D or a Renderbuffer Object)
 * @param count The number of targets for this framebuffer
 * @return Pointer to the created framebuffer, or nullptr if creation fails.
 */
HFX_API PFRAMEBUFFER                HFX_FramebufferCreate(
    usize                       width,
    usize                       height,
    struct FRAMEBUFFER_TARGET*  targets,
    usize                       count);

HFX_API void                        HFX_FramebufferDestroy(
    PFRAMEBUFFER buffer);

HFX_API void                        HFX_FramebufferBind(
    PFRAMEBUFFER framebuffer);

HFX_API void                        HFX_FramebufferResize(
    PFRAMEBUFFER*   framebuffer,
    usize           width,
    usize           height);

HFX_API struct FRAMEBUFFER_TARGET   HFX_FramebufferTargetTexture2D(
    HFX_ENUM attachment,
    HFX_ENUM internal_format,
    HFX_ENUM texture_format,
    HFX_ENUM data_type);

HFX_API struct FRAMEBUFFER_TARGET   HFX_FramebufferTargetRenderbuffer(
    HFX_ENUM attachment,
    HFX_ENUM internal_format);

HFX_API void                        HFX_ElementBufferGenerate(
    EBO* ebo);

HFX_API void                        HFX_ElementBufferBind(
    EBO ebo);

HFX_API void                        HFX_ElementBufferData(
    EBO         ebo,
    const void* data,
    usize       size,
    HFX_ENUM    usage);

HFX_API void                        HFX_VertexBufferGenerate(
    VBO* vbo);

HFX_API void                        HFX_VertexBufferBind(
    VBO vbo);

HFX_API void                        HFX_VertexBufferData(
    VBO         vbo,
    const void* data,
    usize       size,
    HFX_ENUM    usage);

HFX_API void                        HFX_VertexArrayGenerate(
    VAO* vao);

HFX_API void                        HFX_VertexArrayBind(
    VAO vao);

HFX_API void                        HFX_VertexArrayData(
    VAO                         vao,
    struct VERTEX_ATTRIBUTE*    attributes,
    usize                       stride,
    usize                       count);

#endif //HFX_BUFFER_H
