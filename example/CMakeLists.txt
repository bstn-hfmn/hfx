cmake_minimum_required(VERSION 3.30)
project(hfx_example C CXX)

set(CMAKE_C_STANDARD 23)

add_executable(hfx_example main.c ../external/glad/src/glad.c)

target_compile_definitions(hfx_example PRIVATE HFX_SHARED)
target_include_directories(hfx_example PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(hfx_example PUBLIC HFX)

set(SHADERS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/example/shaders")
set(SHADERS_BINARY_DIR "${CMAKE_BINARY_DIR}/example/shaders")

add_custom_command(TARGET hfx_example POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:HFX>              # The HFX DLL file
        $<TARGET_FILE_DIR:hfx_example>  # Copy to executable dir
)

add_custom_target(COPY_SHADERS ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${SHADERS_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/example/shaders")

add_dependencies(hfx_example COPY_SHADERS)