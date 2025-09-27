#include "VBO.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


VBO::VBO(int size, float data[]){
  glGenBuffers(1, &this->ID);  
  glBindBuffer(GL_ARRAY_BUFFER, this->ID);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}