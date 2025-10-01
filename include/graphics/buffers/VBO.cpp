#include "VBO.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


VBO::VBO(){
  glGenBuffers(1, &this->id);
}

void VBO::Unbind(){
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Bind() const{
  glBindBuffer(GL_ARRAY_BUFFER, this->id);
}

void VBO::SetBufferData(int size, float data[]){
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
