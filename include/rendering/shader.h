#ifndef HFX_SHADER_H
#define HFX_SHADER_H

#include "common.h"

#define HFX_SHADER_SOURCE_VERSION "#version 410 core\n"
#define HFX_SHADER_SOURCE_HEADER\
    HFX_SHADER_SOURCE_VERSION\
    "#define hfx_WorldToObject inverse(hfx_uModel)\n"\
    "#define hfx_ObjectToWorld hfx_uModel\n"\
    "#define HFX_WorldToObjectPos(x) vec3(hfx_WorldToObject * vec4(x.xyz, 1.0f))\n"\
    "#define HFX_ObjectToWorldPos(x) vec3(hfx_ObjectToWorld * vec4(x.xyz, 1.0f))\n"\
    "#define HFX_ObjectToClipPos(x) HFX_MATRIX_MVP * vec4(x.xyz, 1.0f)\n"\
    "#define HFX_MATRIX_MVP hfx_uProjection * hfx_uView * hfx_uModel\n"\
    "uniform mat4 hfx_uModel;\n"\
    "uniform mat4 hfx_uView;\n"\
    "uniform mat4 hfx_uProjection;\n"\

#define HFX_SHADER_SOURCE_V2F\
    "\nstruct V2F {\n"\
        "vec3 WorldPosition;\n"\
        "vec4 ClipPosition;\n"\
        "vec3 Normal;\n"\
        "vec2 TexCoord0;\n"\
    "};\n"

#define HFX_SHADER_SOURCE_ATTRIBUTES\
    "\nlayout(location = 0) in vec3 iPosition;\n"\
    "layout(location = 1) in vec3 iNormal;\n"\
    "layout(location = 2) in vec4 iTangent;\n"\
    "layout(location = 3) in vec4 iColor;\n"\
    "layout(location = 4) in vec2 iUV;\n"\
    "\n"\
    "struct ATTRIBUTES {\n"\
        "vec3 Vertex;\n"\
        "vec3 Normal;\n"\
        "vec4 Tangent;\n"\
        "vec4 Color;\n"\
        "vec2 TexCoord0;\n"\
    "};\n"


#define HFX_SHADER_SOURCE_VERT_MAIN\
    "\n#ifdef VERTEX\n"\
    "out V2F VS_OUT;\n"\
    "void main() {\n"\
        "ATTRIBUTES attrs;\n"\
        "attrs.Vertex = iPosition;\n"\
        "attrs.Normal = iNormal;\n"\
        "attrs.Tangent = iTangent;\n"\
        "attrs.Color = iColor;\n"\
        "attrs.TexCoord0 = iUV;\n"\
        "VS_OUT = vert(attrs);\n"\
        "gl_Position = VS_OUT.ClipPosition;\n"\
    "}\n"\
    "#endif\n"\

#define HFX_SHADER_SOURCE_FRAG_MAIN\
    "\n#ifdef FRAGMENT\n"\
    "in V2F VS_OUT;\n"\
    "out vec4 Color;\n"\
    "void main()\n"\
    "{\n"\
        "Color = fragment(VS_OUT);\n"\
    "}\n"\
    "#endif\n"\

#define HFX_SHADER_SOURCE_VERT(x)\
    HFX_SHADER_SOURCE_HEADER,\
    HFX_SHADER_SOURCE_V2F,\
    "#define VERTEX\n",\
    "#ifdef VERTEX\n",\
    HFX_SHADER_SOURCE_ATTRIBUTES,\
    "#endif\n",\
    x,\
    HFX_SHADER_SOURCE_VERT_MAIN,\

#define HFX_SHADER_SOURCE_FRAG(x)\
    HFX_SHADER_SOURCE_HEADER,\
    HFX_SHADER_SOURCE_V2F,\
    "#define FRAGMENT\n",\
    x,\
    HFX_SHADER_SOURCE_FRAG_MAIN\

struct SHADER {
  GL_HANDLE program;
};

typedef struct SHADER *PSHADER, SHADER_T;

HFX_API PSHADER     HFX_ShaderCreate(
    const char* path);

HFX_API void        HFX_ShaderDestroy(
    PSHADER shader);

#endif //HFX_SHADER_H
