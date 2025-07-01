cmake_minimum_required(VERSION 3.30)
project(HFX C CXX)

set(CMAKE_C_STANDARD 23)

add_subdirectory(external/glfw)
add_subdirectory(external/cimgui)

add_library(HFX SHARED
        external/cimgui/cimgui.cpp
        external/cimgui/imgui/imgui.cpp
        external/cimgui/imgui/backends/imgui_impl_opengl3.cpp
        external/cimgui/imgui/backends/imgui_impl_glfw.cpp
        external/glad/src/glad.c
        src/arch/thread.c
        src/video/window.c
        src/logger.c
        src/igw.c
)

add_compile_definitions("IMGUI_IMPL_OPENGL_LOADER_GL3W")
target_compile_definitions(HFX PRIVATE HFX_BUILD)

target_link_libraries(HFX PRIVATE glfw cimgui)

target_include_directories(HFX PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(HFX PUBLIC
    external/glad/include
    external/glfw/include
    external/cglm/include
    external/cimgui)


add_subdirectory(example)