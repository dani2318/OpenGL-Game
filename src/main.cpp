#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

GLFWwindow* window = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

int main(int argc, char **argv) {
  if(!glfwInit()){
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(WIDTH,HEIGHT, "Test", nullptr, nullptr);
  if(!window){
    std::cerr << "Failed create Window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, WIDTH, HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

  while(!glfwWindowShouldClose(window)){
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
