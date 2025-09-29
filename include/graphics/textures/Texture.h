#pragma once
#include <stb_image.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <string>
#include <expected>

class Texture{
    public:
        Texture(std::string texture_path, unsigned int texture_type = GL_TEXTURE_2D);
        
        int Width() const { return width; };
        int Height() const { return height; };
        int NrChannels() const { return nrChannels; };
        unsigned int ID() const { return id; };

        void Bind(unsigned int texture_type = GL_TEXTURE_2D);
        void GenerateTexture();
    private:
        std::expected<bool, std::string> LoadTexture();
        int width, height, nrChannels;
        unsigned char *data;
        unsigned int id;
        unsigned int type;
        std::string texture_path;
};