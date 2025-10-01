#include "Texture.hpp"
#include <iostream>
#include <utils/utilities.hpp>

Texture2D::Texture2D(std::string texture_path)
    : texture_path(texture_path)
{
    glGenTextures(1, &this->id);
}

void Texture2D::Bind(unsigned int tex_id){
    glBindTexture(GL_TEXTURE_2D, tex_id);
}

void Texture2D::Activate(unsigned int texture_unit){
    glActiveTexture(texture_unit);
}

bool Texture2D::LoadTexture(){

    auto exe_dir = GetExecutablePath();

    const auto TEX_PATH = exe_dir.relative_path() / texture_path.c_str();
    const std::string PATH_STR = TEX_PATH.string();
    const char* path = PATH_STR.c_str();

    if (this->data != nullptr) {
        stbi_image_free(this->data);
        this->data = nullptr;
    }

    unsigned char *loaded_data = stbi_load(path, &this->width, &this->height, &this->nr_channels, 0);

    if (loaded_data == nullptr)
    {
        std::string error_msg = "Failed to load texture '" + PATH_STR + "': " + std::string(stbi_failure_reason());
        std::cerr << "STB Error: " << error_msg << '\n';
        return false;
    }
    this->data = loaded_data;

    return true;
}

bool Texture2D::Generate(){

    glGenTextures(1, &this->id);
    this->Bind(GL_TEXTURE0);

    auto load_result = LoadTexture();
    if(!load_result) {
        std::cerr << "Texture generation failed: " << load_result << "\n";
        glDeleteTextures(1, &this->id);
        this->id = 0;
        return false;
    } else {
        std::cout << "Texture loaded correctly from disk: " << this->texture_path << "\n";
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLint internal_format = 0;
    GLenum data_format = 0;

    if (this->nr_channels == 4)
    {
        internal_format = GL_RGBA;
        data_format = GL_RGBA;
    }
    else if (this->nr_channels == 3)
    {
        internal_format = GL_RGB;
        data_format = GL_RGB;
    }
    else {
        std::cerr << "Warning: Unsupported number of channels: " << this->nr_channels << "\n";
    }


    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, this->width, this->height, 0, data_format, GL_UNSIGNED_BYTE, this->data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(this->data);
    this->data = nullptr;

    return true;
}
