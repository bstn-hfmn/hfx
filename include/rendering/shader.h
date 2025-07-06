#ifndef HFX_SHADER_H
#define HFX_SHADER_H

#include "common.h"

#define HFX_SHADER_SOURCE_VERSION "#version 410 core\n"
#define HFX_SHADER_SOURCE_HEADER_VERT\
    HFX_SHADER_SOURCE_VERSION\
    "#define VERTEX\n"\
    "#ifdef VERTEX\n"\
    "#define HFX_MATRIX_MVP hfx_uProjection * hfx_uView * hfx_uModel\n"\
    "uniform mat4 hfx_uModel;\n"\
    "uniform mat4 hfx_uView;\n"\
    "uniform mat4 hfx_uProjection;\n"\
    "#endif\n"

#define HFX_SHADER_SOURCE_HEADER_FRAG\
    HFX_SHADER_SOURCE_VERSION\
    "#define FRAGMENT\n"\

struct SHADER {
  GL_HANDLE program;
};

typedef struct SHADER *PSHADER, SHADER_T;

HFX_API PSHADER     HFX_ShaderCreate(
    const char* path);

HFX_API void        HFX_ShaderDestroy(
    PSHADER shader);

#endif //HFX_SHADER_H
