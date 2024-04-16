#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

engine::Texture::Texture(const Path &filename, GLuint slot)
{
    m_slot = slot;
    glGenTextures(1, &m_id);
    activate();
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename.string().c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        std::cout << filename.extension() << "\n";
        GLenum format = filename.extension().compare(".png") == 0 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    release();
}

engine::Texture::~Texture()
{
    glDeleteTextures(1, &m_id);
}

GLuint engine::Texture::activate()
{
    glActiveTexture(GL_TEXTURE0 + m_slot);
    return m_slot;
}

void engine::Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void engine::Texture::release()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
