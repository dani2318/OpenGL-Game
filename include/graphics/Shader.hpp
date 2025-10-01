#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader{
    public:
        Shader(unsigned int type, const char* source);
        [[nodiscard]] unsigned int GetId() const { return this->id; };
        [[nodiscard]] unsigned int GetType() const { return this->type; };
    private:
        unsigned int id{};
        unsigned int type{};
};


class VertexShader : public Shader {
    public:
        VertexShader(unsigned int type, const char* source);
};

class FragmentShader : public Shader {
    public:
        FragmentShader(unsigned int type, const char* source);
};

class ShaderProgram{

    public:
        void Use() const;
        [[nodiscard]] unsigned int GetId() const {return this->id;};
        ShaderProgram();
        ~ShaderProgram() {
            glDeleteProgram(this->id);
        }
    private:
        unsigned int id;
        VertexShader* vertexshader;
        FragmentShader* fragmentshader;
};
