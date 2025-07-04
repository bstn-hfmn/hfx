#ifndef HFX_SHADER_H
#define HFX_SHADER_H

#include "common.h"

#define HFX_SHADER_SOURCE_DEFINES(x) "#version 410 core\n#define " #x "\n"

struct SHADER {
  GL_HANDLE program;
};

typedef struct SHADER *PSHADER, SHADER_T;

HFX_API PSHADER HFX_ShaderCreate(
    const char* path);

HFX_API void HFX_ShaderDestroy(
    PSHADER shader);

#endif //HFX_SHADER_H
