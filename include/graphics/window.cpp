#include "window.h"

Window::Window(WindowSize size, std::string title){
    this->size = size;
    this->title = title;
    this->window = glfwCreateWindow(this->size.w, this->size.h, this->title.c_str(),NULL, NULL);
    glfwMakeContextCurrent(this->window);
}
