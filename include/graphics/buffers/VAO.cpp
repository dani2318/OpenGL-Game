#include "VAO.hpp"



VAO::VAO(VBO* vbo) : vbo(vbo) {
  glGenVertexArrays(1, &this->id);
  this->Bind();
}

void VAO::Unbind(){
  glBindVertexArray(0);
}

void VAO::Bind() const{
  glBindVertexArray(this->id);
}

void VAO::SetBufferData(int index, int size, int num_vertex_attrib_array, GLsizei stride, GLsizei pointer){
  vbo->Bind();
  glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(pointer * sizeof(float)));
  glEnableVertexAttribArray(num_vertex_attrib_array);
}
