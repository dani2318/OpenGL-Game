#include "Texture.h"

Texture::Texture(std::string texture_path, unsigned int texture_type = GL_TEXTURE_2D){
    data = stbi_load(texture_path.c_str(), &width, &height, &nrChannels, 0); 
}

void Texture::GenerateTexture(){
    glGenTextures(1, &this->id);
    this->Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}