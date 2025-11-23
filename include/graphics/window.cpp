#include "window.hpp"

#include <utility>

Window::Window(WindowSize size, std::string  title) : window_size(size), window_title(std::move(title)), window(glfwCreateWindow(this->window_size.w, this->window_size.h, this->window_title.c_str(), nullptr, nullptr)){
    
    
    
    glfwMakeContextCurrent(this->window);
}
