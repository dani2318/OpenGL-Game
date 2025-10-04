#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct WindowSize {
    int w;
    int h;
};

class Window{
    public:
        Window(WindowSize size, std::string  title);
        [[nodiscard]] WindowSize Size() const { return this->window_size; };
        [[nodiscard]] std::string Title() const { return this->window_title; };
        [[nodiscard]] GLFWwindow* GetWindow() const { return this->window; };
    private:
        WindowSize window_size{};
        std::string window_title;
        GLFWwindow* window = nullptr;
};
