#include "EBO.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
EBO::EBO(){
    glGenBuffers(1, &this->Id);
}

void EBO::Unbind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Bind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id);
}

void EBO::SetBufferData(int size, unsigned int indices[]){
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW); 
}

