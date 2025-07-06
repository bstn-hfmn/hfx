#ifndef HFX_IGW_H
#define HFX_IGW_H

#include "common.h"
#include "rendering/window.h"

HFX_API void    IG_Init(
    PWINDOW window);

HFX_API void    IG_NewFrame();

HFX_API void    IG_Text(
    const char *const text, ...);

HFX_API void    IG_Slider1F(
    const char* label,
    f32 const*  val,
    f32         min,
    f32         max,
    const char* format,
    int         flags);

HFX_API void    IG_Begin(
    const char* name,
    bool*       open,
    int   flags);

HFX_API void    IG_End();

HFX_API void    IG_Render();

HFX_API f32     IG_GetMonitorScale(
    const GLFWmonitor* monitor);

HFX_API void    IG_Destroy();

#endif //HFX_IGW_H
