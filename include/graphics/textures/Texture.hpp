#pragma once
#include <string>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <vendor_include_stb/stb_image.h>

class Texture2D{
    public:
        Texture2D(std::string texture_path);
        static void Bind(unsigned int tex_id);
        static void Activate(unsigned int texture_unit);

        [[nodiscard]] int NrChannels() const { return nr_channels; };
        [[nodiscard]] int Width() const { return width; };
        [[nodiscard]] int Height() const { return height; };
        [[nodiscard]] unsigned int Id() const { return id; };
    private:
        bool LoadTexture();
        bool Generate();
        int width{}, height{}, nr_channels{};
        unsigned char *data = nullptr;
        unsigned int id = -1;
        std::string texture_path;
};
