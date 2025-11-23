#pragma once
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Texture2D{
    public:
        Texture2D(std::string texture_path);
        void Bind() const;
        static void Activate(unsigned int texture_unit);
        bool Generate();

        [[nodiscard]] int NrChannels() const { return nr_channels; };
        [[nodiscard]] int Width() const { return width; };
        [[nodiscard]] int Height() const { return height; };
        [[nodiscard]] unsigned int Id() const { return id; };
    private:
        bool LoadTexture();
        int width{}, height{}, nr_channels{};
        unsigned char *data = nullptr;
        unsigned int id{};
        std::string texture_path;
};
