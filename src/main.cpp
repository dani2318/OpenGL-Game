#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <graphics/Shader.hpp>
#include <graphics/buffers/VBO.hpp>
#include <graphics/buffers/VAO.hpp>
#include <graphics/buffers/EBO.hpp>
#include <graphics/window.hpp>
#include <graphics/textures/Texture.hpp>

struct WindowParameters
{
  WindowSize size;
  const char *title;
};

constexpr WindowParameters WINDOW_PARAMS = {.size={.w=1280, .h=720}, .title="OpenGL game"};

Window *main_window = nullptr;

void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1); //TODO: Change this to show in game pause UI
}

  if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
}

float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
};

unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

int main(int argc, char **argv)
{
  if (glfwInit() == 0)
  {
    std::cerr << "Failed to initialize GLFW" << '\n';
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  std::cout << "Setting up window\n";

  main_window = new Window(WINDOW_PARAMS.size, std::string(WINDOW_PARAMS.title));

  if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0)
  {
    std::cerr << "Failed to initialize GLAD" << '\n';
    return -1;
  }

  glViewport(0, 0, WINDOW_PARAMS.size.w, WINDOW_PARAMS.size.h);
  glfwSetFramebufferSizeCallback(main_window->GetWindow(), FramebufferSizeCallback);

  std::cout << "Setting up graphics buffers\n";

  // Shader setup
  auto vbo = std::make_unique<VBO>();
  auto vao = std::make_unique<VAO>(vbo.get());
  auto ebo = std::make_unique<EBO>();
  auto *shader = new ShaderProgram();

  vao->Bind();
  vbo->Bind();
  vbo->SetBufferData(sizeof(vertices), vertices);
  ebo->Bind();
  ebo->SetBufferData(sizeof(indices), indices);
  vao->SetBufferData(2,2,2,8,6);

  auto* tex = new Texture2D("gamedata\\textures\\testtex.png");
  std::cout << "Starting main loop\n";

  shader->Use();
  tex->Bind(tex->Id());
  glUniform1i(glGetUniformLocation(shader->GetId(), "texture1"), 0); // set it manually

  //Main loop
  while (glfwWindowShouldClose(main_window->GetWindow()) == 0)
  {
    ProcessInput(main_window->GetWindow());

    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->Use();
    tex->Bind(tex->Id());
    tex->Activate(GL_TEXTURE0);
    vao->Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    vao->Unbind();

    glfwSwapBuffers(main_window->GetWindow());
    glfwPollEvents();
  }

  //Cleanup
  delete shader;

  glfwDestroyWindow(main_window->GetWindow());
  delete main_window;

  glfwTerminate();

  return 0;
}
