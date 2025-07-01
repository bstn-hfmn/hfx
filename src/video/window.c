#include <logger.h>
#include <video/window.h>

#include "igw.h"

void framebuffer_size_callback(
    GLFWwindow* window,
    const int width,
    const int height)
{
    const f32 scale = ig_get_monitor_scale(glfwGetPrimaryMonitor());

    glViewport(0, 0, (GLsizei)(scale*(f32)width), (GLsizei)(scale*(f32)height));
    HFX_LOG(LOG_TRACE, "Framebuffer Resized [%dx%d] [IG:Scale %.2f]\n", width, height, scale);
}

void window_init(
    Window *window,
    const char *const title,
    const usize width,
    const usize height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window->title = title;
    window->width = width;
    window->height = height;

    const f32 scale = ig_get_monitor_scale(glfwGetPrimaryMonitor());
    window->handle = glfwCreateWindow((int)((f32)width*scale), (int)((f32)height*scale), title, nullptr, nullptr);
    if (window->handle == nullptr)
    {
        HFX_LOG(LOG_ERROR, "Failed to create GLFW Window\n");
        glfwTerminate();
        return;
    }

    HFX_LOG(LOG_TRACE, "Created Window (OpenGL 4.1; Core Profile)\n");
    glfwMakeContextCurrent(window->handle);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        HFX_LOG(LOG_ERROR, "Failed to load OpenGL using glad.\n");
        glfwTerminate();
        return;
    }

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glfwSetFramebufferSizeCallback(window->handle, framebuffer_size_callback);

    HFX_LOG(LOG_TRACE, "OpenGL (%s)\n", glGetString(GL_VERSION));
    HFX_LOG(LOG_TRACE, "Vendor (%s)\n", glGetString(GL_VENDOR));
    HFX_LOG(LOG_TRACE, "Renderer (%s)\n", glGetString(GL_RENDERER));
    HFX_LOG(LOG_TRACE, "Shaders (%s)\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    ig_init(window);
}

f64 window_get_time()
{
    return glfwGetTime();
}

void window_terminate(
    const Window* window)
{
    if (window != nullptr)
    {
        glfwDestroyWindow(window->handle);
        window = nullptr;
    }

    ig_terminate();
    glfwTerminate();
}

bool window_should_close(
    const Window *const window)
{
    return glfwWindowShouldClose(window->handle);
}


void window_swap_buffers(
    const Window *const window)
{
    glfwSwapBuffers(window->handle);
}

void window_update_input(
    const Window *const window)
{
    glfwPollEvents();
}

void window_clear(
    const f32 red,
    const f32 green,
    const f32 blue,
    const f32 alpha)
{
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}
