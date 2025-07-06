#include "logger.h"
#include "rendering/renderer.h"
#include "rendering/window.h"
#include "rendering/imgui/igw.h"
#include "memory/memory.h"
#include <stdlib.h>

void FramebufferSizeCallback(
    GLFWwindow* window,
    const int width,
    const int height)
{
    const f32 scale = IG_GetMonitorScale(glfwGetPrimaryMonitor());

    GL_CALL(glViewport(0, 0, (GLsizei)(scale*(f32)width), (GLsizei)(scale*(f32)height)));
    HFX_LOG(LOG_TRACE, "Framebuffer Resized [%dx%d] [IG:Scale %.2f]\n", width, height, scale);
}

PWINDOW HFX_WindowCreate(
    const char *const title,
    const usize width,
    const usize height)
{
#ifdef HFX_DEBUG
    HFX_LogInit(stdout);
#endif

    struct WINDOW *const window = HFX_ALLOC(sizeof(struct WINDOW));

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window->title = title;
    window->width = width;
    window->height = height;

    const f32 scale = IG_GetMonitorScale(glfwGetPrimaryMonitor());
    window->handle = glfwCreateWindow((int)((f32)width*scale), (int)((f32)height*scale), title, nullptr, nullptr);
    if (window->handle == nullptr)
    {
        HFX_LOG(LOG_ERROR, "Failed to create GLFW Window\n");
        HFX_SetLastError("Failed to create window");

        glfwTerminate();
        return nullptr;
    }

    HFX_LOG(LOG_TRACE, "Created Window (OpenGL 4.1; Core Profile)\n");
    glfwMakeContextCurrent(window->handle);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        HFX_LOG(LOG_ERROR, "Failed to load OpenGL using glad.\n");
        HFX_SetLastError("Failed to load OpenGL");

        glfwTerminate();
        return nullptr;
    }

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glfwSetFramebufferSizeCallback(window->handle, FramebufferSizeCallback);

    HFX_LOG(LOG_TRACE, "OpenGL (%s)\n", glGetString(GL_VERSION));
    HFX_LOG(LOG_TRACE, "Vendor (%s)\n", glGetString(GL_VENDOR));
    HFX_LOG(LOG_TRACE, "Renderer (%s)\n", glGetString(GL_RENDERER));
    HFX_LOG(LOG_TRACE, "Shaders (%s)\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    HFX_GetRenderer()->window = window;
    return HFX_GetRenderer()->window;
}

void HFX_WindowDestroy(
    PWINDOW window)
{
    glfwDestroyWindow(window->handle);
    glfwTerminate();
}

f64 HFX_WindowGetTime(void)
{
    return glfwGetTime();
}


bool HFX_WindowShouldClose(
    PWINDOW window)
{
    return glfwWindowShouldClose(window->handle);
}


void HFX_WindowSwapBuffers(
    PWINDOW window)
{
    glfwSwapBuffers(window->handle);
}

void HFX_WindowUpdateInput(void)
{
    glfwPollEvents();
}

void HFX_WindowClearScreen(
    const f32 red,
    const f32 green,
    const f32 blue,
    const f32 alpha)
{
    GL_CALL(glClearColor(red, green, blue, alpha));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
