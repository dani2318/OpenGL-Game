#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <graphics/Shader.h>
#include <graphics/buffers/VBO.h>
#include <graphics/buffers/VAO.h>
#include <graphics/buffers/EBO.h>

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

GLFWwindow* window = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right    (0)
     0.5f, -0.5f, 0.0f,  // bottom right (1)
    -0.5f, -0.5f, 0.0f,  // bottom left  (2)
    -0.5f,  0.5f, 0.0f   // top left     (3)
};

unsigned int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

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


  const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

  const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";
  
  VBO* vbo = new VBO(sizeof(vertices),vertices);
  VAO* vao = new VAO(vbo);
  EBO* ebo = new EBO();
  ShaderProgram* shader = new ShaderProgram(vertexShaderSource, fragmentShaderSource);

  vao->Bind();

  vbo->Bind();
  vbo->SetBufferData(sizeof(vertices), vertices);

  ebo->Bind();
  ebo->SetBufferData(sizeof(indices), indices);

  vao->SetBufferData(3);

  std::cout << "VBO ID: " << vbo->getID() << " VAO ID: " << vao->getID() << " EBO ID: " << ebo->getID() << std::endl;

  while(!glfwWindowShouldClose(window)){
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->use();
    vao->Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    vao->Unbind();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
