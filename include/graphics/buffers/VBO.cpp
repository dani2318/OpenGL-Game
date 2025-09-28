#include "VBO.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


VBO::VBO(){
  glGenBuffers(1, &this->ID);
}

void VBO::Unbind(){
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Bind(){
  glBindBuffer(GL_ARRAY_BUFFER, this->ID);
}

void VBO::SetBufferData(int size, float data[]){
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}