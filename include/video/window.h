#ifndef HFX_WINDOW_H
#define HFX_WINDOW_H

#include "common.h"
#include "types.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>


typedef struct {
  const char *title;

  usize width;
  usize height;

  GLFWwindow* handle;
} Window;

HFX_API f64 window_get_time();

HFX_API void window_init(
    Window *window,
    const char *const title,
    const usize width,
    const usize height);

HFX_API void window_terminate(
    const Window* window);

HFX_API bool window_should_close(
    const Window *const window);

HFX_API void window_swap_buffers(
    const Window *const window);

HFX_API void window_update_input(
    const Window *const window);

HFX_API void window_clear(
    f32 red,
    f32 green,
    f32 blue,
    f32 alpha);

#endif //HFX_WINDOW_H
