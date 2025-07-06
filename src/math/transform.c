#include "math/transform.h"

void HFX_TransformGetMatrix(
    struct TRANSFORM* transform,
    mat4 matrix)
{
    mat4 translation = GLM_MAT4_IDENTITY_INIT;
    glm_translate(translation, transform->translation);

    vec3 euler;
    glm_vec3_copy(transform->rotation, euler);
    glm_vec3_scale(euler, GLM_PI / 180.0f, euler);

    versor rot;
    glm_euler_xyz_quat(euler, rot);

    mat4 rotation;
    glm_quat_mat4(rot, rotation);

    mat4 scale = GLM_MAT4_IDENTITY_INIT;
    glm_scale(scale, transform->scale);

    mat4 tr;
    glm_mat4_mul(translation, rotation, tr);
    glm_mat4_mul(tr, scale, matrix);
}
