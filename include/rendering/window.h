#ifndef HFX_WINDOW_H
#define HFX_WINDOW_H

#include "common.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

typedef void(*WindowResizeCallback)(GLFWwindow* window, int width, int height);

struct WINDOW {
  const char *title;

  usize width;
  usize height;

  GLFWwindow* handle;
};

typedef struct WINDOW *PWINDOW, WINDOW_T;

HFX_API PWINDOW HFX_WindowCreate(
        const char*     title,
        usize           width,
        usize           height);

HFX_API void    HFX_WindowDestroy(
        PWINDOW window);

HFX_API f64     HFX_WindowGetTime(void);

HFX_API bool    HFX_WindowShouldClose(
        PWINDOW window);

HFX_API void    HFX_WindowSwapBuffers(
        PWINDOW window);

HFX_API void    HFX_WindowUpdateInput(void);

HFX_API void    HFX_WindowClearScreen(
        f32 red,
        f32 green,
        f32 blue,
        f32 alpha);

#endif //HFX_WINDOW_H
