#ifndef HFX_CAMERA_H
#define HFX_CAMERA_H

#include "common.h"
#include "math/transform.h"

struct CAMERA
{
    struct TRANSFORM transform;
    mat4 projection;
};

typedef struct CAMERA *PCAMERA, CAMERA_T;

void HFX_CameraRecalculateProjection(
    PCAMERA camera);

#endif //HFX_CAMERA_H
