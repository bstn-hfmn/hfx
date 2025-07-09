#include "rendering/shader.h"
#include "memory/memory.h"
#include <stdlib.h>

PSHADER HFX_ShaderCreate(
    const char* path)
{
    FILE* file = fopen(path, "r");
    if (!file)
    {
        HFX_LOG(LOG_ERROR, "Failed to open file to create shader: %s\n", path);
        HFX_SetLastError("Failed to open file");
        return nullptr;
    }

    if (fseek(file, 0, SEEK_END) != 0)
    {
        HFX_LOG(LOG_ERROR, "Failed to seek to end of file\n");
        HFX_SetLastError("Failed to seek to end of file");
        return nullptr;
    }

    const u64 size = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(size + 1);
    const usize read = fread(buffer, sizeof(char), size, file);
    if (read != size) {
        if (ferror(file))
        {
            HFX_LOG(LOG_ERROR, "Failed to read all of the file contents\n");
            HFX_SetLastError("Failed to read all of the file contents");
            return nullptr;
        }
    }

    buffer[read] = '\0';
    fclose(file);

    char* vertexShaderSource[] = {
        HFX_SHADER_SOURCE_VERT(buffer)
    };

    char* fragmentShaderSource[] = {
        HFX_SHADER_SOURCE_FRAG(buffer)
    };

    u32 vertex, fragment;
    GL_CALL(vertex = glCreateShader(GL_VERTEX_SHADER));
    GL_CALL(fragment = glCreateShader(GL_FRAGMENT_SHADER));

    GL_CALL(glShaderSource(vertex, 8, (const GLchar**)vertexShaderSource, nullptr));
    GL_CALL(glShaderSource(fragment, 5, (const GLchar**)fragmentShaderSource, nullptr));
    HFX_FREE(buffer);

    GL_CALL(glCompileShader(vertex));

    char info[512];
    GLint vertexShaderSuccess;
    GL_CALL(glGetShaderiv(vertex, GL_COMPILE_STATUS, &vertexShaderSuccess));
    if (vertexShaderSuccess)
    {
        GL_CALL(glCompileShader(fragment));

        GLint fragmentShaderSuccess;
        GL_CALL(glGetShaderiv(fragment, GL_COMPILE_STATUS, &fragmentShaderSuccess));
        if (fragmentShaderSuccess)
        {
            GLint program;
            GL_CALL(program = glCreateProgram());
            GL_CALL(glAttachShader(program, vertex));
            GL_CALL(glAttachShader(program, fragment));
            GL_CALL(glLinkProgram(program));

            GLint progamLinkSuccess;
            glGetProgramiv(program, GL_LINK_STATUS, &progamLinkSuccess);
            if (progamLinkSuccess)
            {
                GL_CALL(glDeleteShader(vertex));
                GL_CALL(glDeleteShader(fragment));

                struct SHADER* shader = HFX_ALLOC(sizeof(struct SHADER));
                shader->program = program;
                return shader;
            } else {
                GL_CALL(glGetShaderInfoLog(fragment, 512, nullptr, info));
                HFX_LOG(LOG_ERROR, "Failed to link program (%s)\n\t: %s\n", path, info);

                HFX_SetLastError("Failed to link shader program");
            }
        } else {
            GL_CALL(glGetShaderInfoLog(fragment, 512, nullptr, info));
            HFX_LOG(LOG_ERROR, "Failed to compile fragment shader (%s)\n\t: %s\n", path, info);
            HFX_SetLastError("Failed to compile fragment shader");
        }
    } else {
        GL_CALL(glGetShaderInfoLog(vertex, 512, nullptr, info));
        HFX_LOG(LOG_ERROR, "Failed to compile vertex shader (%s)\n\t: %s\n", path, info);
        HFX_SetLastError("Failed to compile vertex shader");
    }

    if (vertex != GL_NONE)
        glDeleteShader(vertex);
    if (fragment != GL_NONE)
        glDeleteShader(fragment);

    return nullptr;
}

void HFX_ShaderDestroy(
    PSHADER shader)
{
    HFX_FREE(shader);
}
