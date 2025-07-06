#include <rendering/buffer.h>
#include <rendering/mesh.h>
#include <rendering/imgui/igw.h>
#include <rendering/window.h>
#include <rendering/shader.h>

#include <fileapi.h>
#include <rendering/renderer.h>

int main() {
    if (!HFX_Init())
    {
        fprintf(stderr, "Failed to initialize HFX\n");
        exit(1);
    }

    struct WINDOW* window = HFX_WindowCreate("Hello World", 1200, 675);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to create window (Error: %s)\n", HFX_GetLastError());
        exit(1);
    }

    struct MESH* cube = HFX_MeshCreatePrimitive(HFX_PRIMITIVE_CUBE);

    char buffer[64];
    GetFullPathName("shaders/example.glsl", 64, buffer, nullptr);

    struct SHADER* shader = HFX_ShaderCreate(buffer);
    if (shader == NULL)
    {
        fprintf(stderr, "Failed to create shader (Error: %s)\n", HFX_GetLastError());
        exit(1);
    }

    struct TRANSFORM transform;
    glm_vec3_make((vec3){ 0, 0, -3.0f }, transform.translation);
    glm_vec3_make((vec3){ 0, 5.0f, 0 }, transform.rotation);
    glm_vec3_make((vec3){ 1, 1, 1 }, transform.scale);

    IG_Init(window);
    while (!HFX_WindowShouldClose(window))
    {
        HFX_WindowUpdateInput();

        float time = (float)HFX_WindowGetTime();
        transform.rotation[1] = time * 12.0f;

        HFX_WindowClearScreen(0.125f, 0.125f, 0.125f, 1.0f);
        { // Geometry

            /*HFX_CmdCullMode(true, HFX_CULL_BACK, HFX_POLYGON_FILL);
            HFX_CmdDepthMode(HFX_DEPTH_READ_WRITE_ENABLE);
            HFX_CmdDepthFunc(HFX_DEPTH_LESS);

            HFX_CmdDraw();*/

            //HFX_RendererDrawMesh(); submits the cmd buffer
            HFX_RendererDrawMesh(cube, shader, transform);
            //HFX_DrawMesh(cube, shader);
        }

        IG_NewFrame();
        { // UI
            IG_Begin("Sample Window", nullptr, 0);
            IG_Text("This is useful, I guess..");
            IG_End();
        }
        IG_Render();

        HFX_WindowSwapBuffers(window);
    }

    IG_Destroy();
    HFX_Destroy();
    return 0;
}
