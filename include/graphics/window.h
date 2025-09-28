#pragma once
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct WindowSize {
    int w;
    int h;
};

class Window{
    public:
        Window(WindowSize size, std::string title);
        WindowSize Size() const { return this->size; };
        std::string Title() const { return this->title; };
        GLFWwindow* getWindow() const { return this->window; };
    private:
        WindowSize size;
        std::string title;
        GLFWwindow* window = nullptr;
};