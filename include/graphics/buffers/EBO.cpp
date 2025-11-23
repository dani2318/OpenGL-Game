#include "EBO.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

EBO::EBO(){
    glGenBuffers(1, &this->id);
}

void EBO::Unbind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Bind() const{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
}

void EBO::SetBufferData(int size, unsigned int indices[]){
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}
