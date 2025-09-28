#include "VAO.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


VAO::VAO(VBO* vbo) : vbo(vbo) {
  glGenVertexArrays(1, &this->Id);
  Bind();
}

void VAO::Unbind(){
  glBindVertexArray(0);
}

void VAO::Bind(){
  glBindVertexArray(this->Id);
}

void VAO::SetBufferData(int size){
  vbo->Bind();
  glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
}