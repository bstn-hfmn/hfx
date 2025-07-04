#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_USE_OPENGL3
#define CIMGUI_USE_GLFW

#include <cimgui.h>
#include <cimgui_impl.h>

#include "rendering/imgui/igw.h"

void IG_Init(
    const PWINDOW window)
{
    const float scale = IG_GetMonitorScale(glfwGetPrimaryMonitor());
    igCreateContext(nullptr);

    ImGuiIO *io = igGetIO_Nil();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls

    ImGuiStyle* style = igGetStyle();
    ImGuiStyle_ScaleAllSizes(style, scale);
    style->FontScaleDpi = scale;
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
    io->ConfigDpiScaleFonts = true;
    io->ConfigDpiScaleViewports = true;
#endif

    ImGui_ImplGlfw_InitForOpenGL(window->handle, true);
    ImGui_ImplOpenGL3_Init("#version 410 core");

    igStyleColorsDark(nullptr);
}


f32 IG_GetMonitorScale(const GLFWmonitor *const monitor)
{
    return ImGui_ImplGlfw_GetContentScaleForMonitor((GLFWmonitor*)monitor);
}

void IG_NewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();
}

void IG_Begin(
    const char* name,
    bool* open,
    const int flags)
{
    igBegin(name, open, flags);
}

void IG_Text(const char *const text, ...)
{
    va_list args;
    va_start(args, text);
    igTextV(text, args);
    va_end(args);
}


void IG_Slider1F(
    const char* label,
    f32 const* val,
    const f32 min,
    const f32 max,
    const char* format,
    const int flags)
{
    igSliderFloat(label, (f32*)val, min, max, format, flags);
}

void IG_End()
{
    igEnd();
}

void IG_Render()
{
    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

void IG_Destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(nullptr);
}