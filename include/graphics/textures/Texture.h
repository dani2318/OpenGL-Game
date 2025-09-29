#pragma once
#include <stb_image.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <string>

class Texture{
    public:
        Texture(std::string texture_path, unsigned int texture_type = GL_TEXTURE_2D);
        int Width() const { return width; };
        int Height() const { return height; };
        int NrChannels() const { return nrChannels; };
        unsigned int ID() const { return id; };

        void Bind() const { glBindTexture(GL_TEXTURE_2D, ID()); };
        void GenerateTexture();
    private:
        int width, height, nrChannels;
        unsigned char *data;
        unsigned int id;
        unsigned int type;
};