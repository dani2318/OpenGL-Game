#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Shader::Shader(unsigned int type, const char* source)
    : Id(glCreateShader(type)), Source(source), Type(type){
    glShaderSource(this->Id, 1, &Source, NULL);
    glCompileShader(this->Id);
}


VertexShader::VertexShader(unsigned int type, const char* source) : Shader(type, source){

}


FragmentShader::FragmentShader(unsigned int type, const char* source) : Shader(type, source){

}


void ShaderProgram::use(){
    glUseProgram(this->Id);
}

ShaderProgram::ShaderProgram(const char* vertexSource, const char* fragmentSource)
    : Id(glCreateProgram()){

    this->Vertexshader = new VertexShader(GL_VERTEX_SHADER, vertexSource);
    this->Fragmentshader = new FragmentShader(GL_FRAGMENT_SHADER, fragmentSource);

    glAttachShader(this->Id, this->Vertexshader->getID());
    glAttachShader(this->Id, this->Fragmentshader->getID());

    glLinkProgram(this->Id);
    use();

    glDeleteShader(this->Vertexshader->getID());
    glDeleteShader(this->Fragmentshader->getID());

}

