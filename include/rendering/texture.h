#ifndef HFX_TEXTURE_H
#define HFX_TEXTURE_H

#include "common.h"

#define HFX_TEXTURE_KEEP_WIDTH -1
#define HFX_TEXTURE_KEEP_HEIGHT -1

struct TEXTURE {
  usize width, height;
  GL_HANDLE handle;
};

typedef struct TEXTURE *PTEXTURE, TEXTURE_T;

HFX_API PTEXTURE  HFX_TextureCreate(
  const char* path,
  bool        mipmaps);

HFX_API void      HFX_TextureDestroy(
  PTEXTURE texture);

#endif //HFX_TEXTURE_H
