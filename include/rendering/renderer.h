#ifndef HFX_RENDERER_H
#define HFX_RENDERER_H

#include "common.h"
#include "rendering/mesh.h"
#include "rendering/window.h"
#include "rendering/camera.h"
#include "rendering/texture.h"

#define COMMAND_BUFFER_CAPACITY 256

enum
{
  HFX_COMMAND_BIND_VERTEX_BUFFER,
  HFX_COMMAND_BIND_INDEX_BUFFER,
  HFX_COMMAND_BIND_VERTEX_ARRAY,
  HFX_COMMAND_BIND_SHADER,

  HFX_COMMAND_SET_VIEWPORT,

  HFX_COMMAND_DRAW,
  HFX_COMMAND_CLEAR,
};

struct COMMAND
{
  HFX_ENUM command;

  union {
    struct { GL_HANDLE handle; } bind;
    struct { GLbitfield mask; } clear;
    struct { GLint x, y, width, height; } viewport;
    struct { GLenum mode; GLsizei count; GLenum type; } draw;
  };
};

struct COMMAND_BUFFER
{
  struct COMMAND commands[COMMAND_BUFFER_CAPACITY];
  usize commandCount;
};

struct RENDERER {
  struct CAMERA* camera;
  struct WINDOW* window;
};

typedef struct RENDERER *PRENDERER, RENDERER_T;
typedef struct HFX_COMMAND *PCOMMAND, COMMAND_T;
typedef struct COMMAND_BUFFER *PCOMMAND_BUFFER, COMMAND_BUFFER_T;

HFX_API bool        HFX_Init(
  const char* title,
  usize       width,
  usize       height);

HFX_API void        HFX_Destroy();

HFX_API PCAMERA     HFX_GetCamera();
HFX_API PWINDOW     HFX_GetWindow();
HFX_API PRENDERER   HFX_GetRenderer();

HFX_API void        HFX_RendererDrawMesh(
    PMESH             mesh,
    PSHADER           shader,
    struct TRANSFORM  transform,
    PTEXTURE          texture);

#endif //HFX_RENDERER_H
