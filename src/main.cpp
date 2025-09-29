#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <graphics/Shader.h>
#include <graphics/buffers/VBO.h>
#include <graphics/buffers/VAO.h>
#include <graphics/buffers/EBO.h>
#include <graphics/window.h>

struct WindowParameters
{
  WindowSize size;
  const char *title;
};

constexpr WindowParameters windowParams = {{1280, 720}, "OpenGL game"};

Window *main_window = nullptr;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true); //TODO: Change this to show in game pause UI

  if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

float vertices[] = {
    0.5f, 0.5f, 0.0f,   // top right    (0)
    0.5f, -0.5f, 0.0f,  // bottom right (1)
    -0.5f, -0.5f, 0.0f, // bottom left  (2)
    -0.5f, 0.5f, 0.0f   // top left     (3)
};

unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

int main(int argc, char **argv)
{
  if (!glfwInit())
  {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  main_window = new Window(windowParams.size, windowParams.title);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, windowParams.size.w, windowParams.size.h);
  glfwSetFramebufferSizeCallback(main_window->getWindow(), framebuffer_size_callback);

  // Shader setup
  auto vbo = std::make_unique<VBO>();
  auto vao = std::make_unique<VAO>(vbo.get());
  auto ebo = std::make_unique<EBO>();
  ShaderProgram *shader = new ShaderProgram();

  vao->Bind();
  vbo->Bind();
  vbo->SetBufferData(sizeof(vertices), vertices);
  ebo->Bind();
  ebo->SetBufferData(sizeof(indices), indices);
  vao->SetBufferData(3);


  //Texture setup
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //Main loop
  while (!glfwWindowShouldClose(main_window->getWindow()))
  {
    processInput(main_window->getWindow());

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->use();
    vao->Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    vao->Unbind();

    glfwSwapBuffers(main_window->getWindow());
    glfwPollEvents();
  }

  //Cleanup
  delete shader;
  delete main_window;

  glfwDestroyWindow(main_window->getWindow());
  glfwTerminate();

  return 0;
}
