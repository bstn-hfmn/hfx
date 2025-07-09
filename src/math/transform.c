#include "math/transform.h"

void HFX_TransformGetToWorldMatrix(
    struct TRANSFORM* transform,
    mat4 matrix)
{
    mat4 translation = GLM_MAT4_IDENTITY_INIT;
    glm_translate(translation, transform->translation);

    vec3 euler;
    glm_vec3_copy(transform->rotation, euler);
    glm_vec3_scale(euler, GLM_PIf / 180.0f, euler);

    versor rot;
    glm_euler_xyz_quat(euler, rot);

    mat4 rotation = GLM_MAT4_IDENTITY_INIT;
    glm_quat_mat4(rot, rotation);

    mat4 scale = GLM_MAT4_IDENTITY_INIT;
    glm_scale(scale, transform->scale);

    glm_mat4_mul(translation, rotation, matrix);
    glm_mat4_mul(matrix, scale, matrix);
}

void HFX_TransformGetToLocalMatrix(
    struct TRANSFORM* transform,
    mat4 matrix)
{
    HFX_TransformGetToWorldMatrix(transform, matrix);
    glm_mat4_inv(matrix, matrix);
}

void HFX_TransformGetForward(struct TRANSFORM* transform, vec3 forward)
{
    vec3 euler;
    glm_vec3_scale(transform->rotation, TO_RADIANS, euler);

    versor quat;
    glm_euler_xyz_quat(euler, quat);

    mat4 rot;
    glm_quat_mat4(quat, rot);
    glm_mat4_mulv3(rot, GLM_ZUP, 1.0f, forward);
}

void HFX_TransformLookAt(
    struct TRANSFORM* transform,
    vec3 target,
    vec3 up)
{
    mat4 rot;
    glm_lookat(transform->translation, target, up, rot);
    glm_mat4_inv(rot, rot);

    glm_euler_angles(rot, transform->rotation);
    glm_vec3_scale(transform->rotation, TO_DEGREES, transform->rotation);
}

struct TRANSFORM HFX_TransformFromPosition(
    vec3 position)
{
    struct TRANSFORM transform;
    glm_vec3_copy(position, transform.translation);
    glm_vec3_zero(transform.rotation);
    glm_vec3_one(transform.scale);

    return transform;
}
