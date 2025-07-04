#include <rendering/buffer.h>
#include <rendering/mesh.h>
#include <rendering/imgui/igw.h>
#include <rendering/window.h>
#include <rendering/shader.h>

#include <fileapi.h>

int main() {
    struct WINDOW* window = HFX_WindowCreate("Hello World", 1200, 675);
    if (window == NULL) {
        fprintf(stderr, "Failed to create window (Error: %d)\n", HFX_GetLastError());
        exit(1);
    }

    struct MESH* mesh = HFX_MeshCreate(3, 3);
    HFX_MeshSetVertexPositions(mesh, 0, 3, (vec3[]) {
        { -0.5f, -0.5f, 0.0f },
        { 0.5f, -0.5f, 0.0f },
        { 0.0f, 0.5f, 0.0f }
    });
    HFX_MeshSetIndices(mesh, 0, 3, (u32[]) { 0, 1, 2 });
    HFX_MeshUploadBuffers(mesh);

    char buffer[64];
    GetFullPathName("shaders/example.glsl", 64, buffer, nullptr);

    struct SHADER* shader = HFX_ShaderCreate(buffer);
    if (shader == NULL)
    {
        fprintf(stderr, "Failed to create shader (Error: %d)\n", HFX_GetLastError());
        exit(1);
    }

    IG_Init(window);
    while (!HFX_WindowShouldClose(window))
    {
        HFX_WindowUpdateInput();
        IG_NewFrame();

        HFX_WindowClearScreen(0.125f, 0.125f, 0.125f, 1.0f);
        { // Geometry
            HFX_DrawMesh(mesh, shader);
        }

        { // UI
            IG_Begin("Sample Window", nullptr, 0);
            IG_Text("This is useful, I guess..");
            IG_End();
        }

        IG_Render();
        HFX_WindowSwapBuffers(window);
    }

    IG_Destroy();

    HFX_MeshDestroy(mesh);
    HFX_ShaderDestroy(shader);
    HFX_WindowDestroy(window);
    return 0;
}
