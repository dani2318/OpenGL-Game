#include "Texture.h"

Texture::Texture(std::string texture_path, unsigned int texture_type = GL_TEXTURE_2D)
    : texture_path(texture_path)
{
    glGenTextures(1, &this->id);
    this->LoadTexture();
    this->Bind(texture_type)
}

void Texture::Bind(unsigned int texture_type = GL_TEXTURE_2D)
{
    glBindTexture(GL_TEXTURE_2D, ID());
}

std::expected<bool, std::string> Texture::LoadTexture()
{
    unsigned char *data = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::unexpected("Failed to load texture");
        return false;
    }
    this->data = data;
    return true;
}

void Texture::GenerateTexture()
{
    glGenTextures(1, &this->id);
    this->Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->data);
    glGenerateMipmap(GL_TEXTURE_2D);
}