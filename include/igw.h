#ifndef HFX_IGW_H
#define HFX_IGW_H

#include "types.h"
#include "common.h"
#include "video/window.h"

HFX_API void ig_init(const Window *const window);

HFX_API void ig_frame_new();

HFX_API void ig_text(const char *const text, ...);

HFX_API void ig_slider1f(
    const char* label,
    f32 const* val,
    const f32 min,
    const f32 max,
    const char* format,
    const int flags);

HFX_API void ig_begin(
    const char* name,
    bool* open,
    const int flags);

HFX_API void ig_end();

HFX_API void ig_render();

HFX_API f32 ig_get_monitor_scale(const GLFWmonitor *const monitor);

HFX_API void ig_terminate();

#endif //HFX_IGW_H
