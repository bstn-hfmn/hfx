#ifndef HFX_COMMON_H
#define HFX_COMMON_H

#include <stdint.h>
#include <glad/glad.h>

#define HFX_NONE 0
#define PIf ((float)3.14159265358979323846264338327950288)
#define TO_DEGREES 180.0f / PIf
#define TO_RADIANS PIf / 180.0f

#define HFX_FREE(x) if(x != nullptr) { free(x); x = nullptr; }

#ifdef HFX_BUILD
    #define HFX_API __declspec(dllexport)
#else
      #define HFX_API __declspec(dllimport)
#endif

#if (defined(DEBUG) || defined(_DEBUG)) || !defined(NDEBUG)
    #include "logger.h"
    #define HFX_LOG(level, format, ...)\
        HFX_Log("[%s][%s:%d]: " format, HFX_LogLevelCStr(level), __FILE__, __LINE__, ##__VA_ARGS__)

    #define GL_CALL(x)\
        do {\
            x;\
            GLenum err;\
            while ((err = glGetError()) != GL_NO_ERROR) {\
                HFX_LOG(LOG_ERROR, "OpenGL Error (%d)\n\t(%s)\n", err, #x);\
            }\
        }\
        while (0)
#else
    #define GL_CALL(x) x
    #define HFX_LOG(level, format, ...) HFX_SetLastError()
#endif

typedef typeof((int*)nullptr - (int*)nullptr) ptrdiff_t;

typedef size_t usize;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float f32;
typedef double f64;

typedef u32 HFX_ENUM;
typedef u32 GL_HANDLE;

enum
{
  HFX_TYPE_UNK = 0,
  HFX_TYPE_UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
  HFX_TYPE_BYTE = GL_BYTE,
  HFX_TYPE_UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
  HFX_TYPE_SHORT = GL_SHORT,
  HFX_TYPE_UNSIGNED_INT = GL_UNSIGNED_INT,
  HFX_TYPE_INT = GL_INT,
  HFX_TYPE_HALF_FLOAT = GL_HALF_FLOAT,
  HFX_TYPE_FLOAT = GL_FLOAT,
  HFX_TYPE_UINT_24_8 = GL_UNSIGNED_INT_24_8,
};

enum
{
  HFX_FORMAT_UNK = 0,
  HFX_FORMAT_DEPTH_32F = GL_DEPTH_COMPONENT32F,
  HFX_FORMAT_DEPTH24_STENCIL_8 = GL_DEPTH24_STENCIL8,

  HFX_FORMAT_RED = GL_RED,
  HFX_FORMAT_RG = GL_RG,
  HFX_FORMAT_RGB = GL_RGB,
  HFX_FORMAT_BGR = GL_BGR,
  HFX_FORMAT_BGRA = GL_BGRA,
  HFX_FORMAT_RGBA = GL_RGBA,
  HFX_FORMAT_DEPTH = GL_DEPTH_COMPONENT,
  HFX_FORMAT_DEPTH_STENCIL = GL_DEPTH_STENCIL,

  HFX_FORMAT_R8 = GL_R8,
  HFX_FORMAT_R16 = GL_R16,
  HFX_FORMAT_RGB4 = GL_RGB4,
  HFX_FORMAT_RGB8 = GL_RGB8,
  HFX_FORMAT_RGBA4 = GL_RGBA4,
  HFX_FORMAT_RGBA8 = GL_RGBA8,

  HFX_FORMAT_R32F = GL_R32F,
  HFX_FORMAT_RG32F = GL_RG32F,
  HFX_FORMAT_RGB32F = GL_RGB32F,
  HFX_FORMAT_RGBA32F = GL_RGBA32F,
};

void                HFX_SetLastError(
    const char* error);

HFX_API const char*   HFX_GetLastError();

#endif //HFX_COMMON_H
