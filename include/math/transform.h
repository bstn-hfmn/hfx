#ifndef HFX_TRANSFORM_H
#define HFX_TRANSFORM_H

#include "common.h"
#include <cglm/cglm.h>

struct TRANSFORM
{
  vec3 translation;
  vec3 rotation;
  vec3 scale;
};

typedef struct TRANSFORM *PTRANSFORM, TRANSFORM_T;

HFX_API void HFX_TransformGetMatrix(
  struct TRANSFORM* transform,
  mat4              matrix);

#endif //HFX_TRANSFORM_H
