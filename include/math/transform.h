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

HFX_API void              HFX_TransformGetToWorldMatrix(
  struct TRANSFORM* transform,
  mat4              matrix);

HFX_API void              HFX_TransformGetToLocalMatrix(
  struct TRANSFORM* transform,
  mat4              matrix);

HFX_API void              HFX_TransformGetForward(
  struct TRANSFORM* transform,
  vec3 forward);

HFX_API void              HFX_TransformLookAt(
  struct TRANSFORM* transform,
  vec3              target,
  vec3              up);

HFX_API struct TRANSFORM  HFX_TransformFromPosition(
  vec3 position);

#endif //HFX_TRANSFORM_H
