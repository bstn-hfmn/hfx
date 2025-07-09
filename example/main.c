#include <rendering/buffer.h>
#include <rendering/mesh.h>
#include <rendering/imgui/igw.h>
#include <rendering/window.h>
#include <rendering/shader.h>

#include <fileapi.h>
#include <rendering/renderer.h>

int main() {
    if (!HFX_Init("Example", 1280, 720))
    {
        fprintf(stderr, "Failed to initialize HFX\n");
        exit(1);
    }

    struct MESH* cube = HFX_MeshPrimitiveCreate(HFX_PRIMITIVE_CUBE);

    char buffer[64];
    GetFullPathName("shaders/example.glsl", 64, buffer, nullptr);

    struct SHADER* shader = HFX_ShaderCreate(buffer);
    if (shader == NULL)
    {
        fprintf(stderr, "Failed to create shader (Error: %s)\n", HFX_GetLastError());
        exit(1);
    }

    struct TRANSFORM transforms[5] = {
        HFX_TransformFromPosition((vec3) { 0, 0, -5.f }),
        HFX_TransformFromPosition((vec3) { 2, 0, -7.f }),
        HFX_TransformFromPosition((vec3) { -3, 1, -4.f }),
        HFX_TransformFromPosition((vec3) { 1, -3, -8.f }),
        HFX_TransformFromPosition((vec3) { 4, 3, -6.f }),
    };

    GetFullPathName("textures/base-map.png", 64, buffer, nullptr);
    struct TEXTURE* texture = HFX_TextureCreate(buffer, false);

    IG_Init(HFX_GetWindow());
    while (!HFX_WindowShouldClose(HFX_GetWindow()))
    {
        HFX_WindowUpdateInput();
        HFX_WindowClearScreen(0.125f, 0.125f, 0.125f, 1.0f);
        {
            for (int i = 0; i < 5; i++)
            {
                HFX_RendererDrawMesh(cube, shader, transforms[i], texture);
            }
        }

        IG_NewFrame();
        { // UI
            IG_Begin("Transforms", nullptr, 0);

            for (int i = 0; i < 5; i++)
            {
                char label[32];
                snprintf(label, 32, "Cube %d", i);

                char rot[32];
                snprintf(rot, 32, "##Rotation-%d", i);

                char loc[32];
                snprintf(loc, 32, "##Position-%d", i);

                if (IG_CollapsingHeader(label))
                {
                    IG_Text("Rotation");
                    IG_Slider3F(rot, transforms[i].rotation, 0.f, 360.f, "%.1f °", 0);

                    IG_Text("Position");
                    IG_Slider3F(loc, transforms[i].translation, -5.f, 5.f, "%.1f", 0);

                    if (IG_Button("Reset"))
                    {
                        glm_vec3_zero(transforms[i].translation);
                        glm_vec3_zero(transforms[i].rotation);
                    }
                }
            }

            IG_End();
        }
        IG_Render();

        HFX_WindowSwapBuffers(HFX_GetWindow());
    }

    IG_Destroy();
    HFX_Destroy();
    return 0;
}
