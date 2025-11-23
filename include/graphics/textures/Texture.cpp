#include "Texture.hpp"
#include <string>
#include <utility>
#include <utils/utilities.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <utils/debug.hpp>
#define MODULE_NAME "Texture"

Texture2D::Texture2D(std::string texture_path)
    : texture_path(std::move(std::move(texture_path)))
{
    glGenTextures(1, &this->id);
}

void Texture2D::Bind() const{
    glBindTexture(GL_TEXTURE_2D, this->Id());
}

void Texture2D::Activate(unsigned int texture_unit){
    glActiveTexture(texture_unit);
}

bool Texture2D::LoadTexture(){

    auto exe_path = GetExecutablePath();
    const auto TEX_PATH = exe_path / texture_path.c_str();
    Debug::Info(MODULE_NAME, (std::string("Attempting to load texture from: ") + TEX_PATH.string()).c_str());
    if (this->data != nullptr) {
        stbi_image_free(this->data);
        this->data = nullptr;
    }
    stbi_set_flip_vertically_on_load(1);
    unsigned char *loaded_data = stbi_load(TEX_PATH.string().c_str(), &this->width, &this->height, &this->nr_channels, 0);

    if (loaded_data == nullptr)
    {
        Debug::Critical(MODULE_NAME, (std::string("Failed to load texture ") + TEX_PATH.string() + ":" + std::string(stbi_failure_reason())).c_str());
        return false;
    }
    this->data = loaded_data;

    return true;
}

bool Texture2D::Generate(){

    glGenTextures(1, &this->id);
    this->Bind();

    auto load_result = LoadTexture();
    if(!load_result) {
        Debug::Critical(MODULE_NAME, (std::string("Texture generation failed")).c_str());
        glDeleteTextures(1, &this->id);
        this->id = 0;
        return false;
    } else {
        Debug::Info(MODULE_NAME, (std::string("Texture loaded correctly from disk:") + this->texture_path).c_str());
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint internal_format = 0;
    GLenum data_format = 0;

    if (this->nr_channels == 4) {
        internal_format = GL_RGBA;
        data_format = GL_RGBA;
    } else if (this->nr_channels == 3) {
        internal_format = GL_RGB;
        data_format = GL_RGB;
    } else if (this->nr_channels == 1) {
        internal_format = GL_RED;
        data_format = GL_RED;
    } else {
        Debug::Critical(MODULE_NAME, (std::string("Error: Unsupported number of channels: ") + std::to_string(this->nr_channels)).c_str());
        // *** You should fail the generation here! ***
        glDeleteTextures(1, &this->id);
        this->id = 0;
        stbi_image_free(this->data);
        this->data = nullptr;
        return false;
    }


    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, this->width, this->height, 0, data_format, GL_UNSIGNED_BYTE, this->data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(this->data);
    this->data = nullptr;

    return true;
}
