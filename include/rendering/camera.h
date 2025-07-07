#ifndef HFX_CAMERA_H
#define HFX_CAMERA_H

#include "common.h"
#include "math/transform.h"

struct CAMERA
{
    struct TRANSFORM transform;

    f32 fov;
    f32 aspect;
    f32 near;
    f32 far;
    mat4 projection;
};

typedef struct CAMERA *PCAMERA, CAMERA_T;

#endif //HFX_CAMERA_H
