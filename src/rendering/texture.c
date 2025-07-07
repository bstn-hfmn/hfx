#include "rendering/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <memory/memory.h>


PTEXTURE HFX_TextureCreate(
    const char* path,
    bool mipmaps)
{
    u32 channels;

    usize width, height;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path, (int*)&width, (int*)&height, (int*)&channels, 4);
    if (!data)
    {
        HFX_LOG(LOG_ERROR, "Failed to load texture from %s\n", path);
        HFX_SetLastError("Failed to load texture");
        return nullptr;
    }


    GLuint handle;
    GL_CALL(glGenTextures(1, &handle));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, handle));
    GL_CALL(glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        (GLint)width,
        (GLint)height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data));

    stbi_image_free(data);
    if (mipmaps)
        GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    struct TEXTURE* texture = HFX_ALLOC(sizeof(struct TEXTURE));
    texture->handle = handle;
    texture->width = width;
    texture->height = height;
    return texture;
}

void HFX_TextureDestroy(
    PTEXTURE texture)
{
    glDeleteTextures(1, &texture->handle);
}


