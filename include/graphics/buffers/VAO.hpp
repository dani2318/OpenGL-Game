#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "VBO.hpp"

class VAO{
    public:
        VAO(VBO* vbo);

        static void Unbind();
        void Bind() const;
        void SetBufferData(int index, int size, GLsizei stride, GLsizei pointer);

        [[nodiscard]] unsigned int GetId() const { return this->id; };
    private:
        unsigned int id{};
        VBO* vbo = nullptr;
};
