#ifndef HFX_CAMERA_H
#define HFX_CAMERA_H

#include "common.h"
#include "math/transform.h"

struct CAMERA
{
    struct TRANSFORM transform;

    mat4 projection;
    f32 fov;
    f32 aspect;
    f32 near;
    f32 far;
};

typedef struct CAMERA *PCAMERA, CAMERA_T;

#endif //HFX_CAMERA_H
