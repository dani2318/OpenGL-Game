#pragma once
class Shader{
    public:
        Shader(unsigned int type, const char* source);
        unsigned int getID() const { return this->Id; };
        unsigned int getType() const { return this->Type; };
    private:
        unsigned int Id;
        const char * Source;
        unsigned int Type;
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
        void use();
        ShaderProgram(const char* vertexSource, const char* fragmentSource);
    private:
        unsigned int Id;
        VertexShader* Vertexshader;
        FragmentShader* Fragmentshader;
};

