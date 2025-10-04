#include <array>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <graphics/Shader.hpp>
#include <graphics/buffers/EBO.hpp>
#include <graphics/buffers/VAO.hpp>
#include <graphics/buffers/VBO.hpp>
#include <graphics/camera/Camera.hpp>
#include <graphics/textures/Texture.hpp>
#include <graphics/window.hpp>

#include <utils/debug.hpp>
#define MODULE_NAME "Main"

struct WindowParameters {
  WindowSize size;
  const char *title;
};

constexpr WindowParameters WINDOW_PARAMS = {.size = {.w = 1280, .h = 720},
                                            .title = "OpenGL game"};
Camera *camera = new Camera();

glm::mat4 proj = glm::perspective(
    glm::radians(45.0F), float(WINDOW_PARAMS.size.w) / WINDOW_PARAMS.size.h,
    0.1F, 100.0F);
glm::mat4 model = glm::mat4(1.0F);

float delta_time = 0.0F; // Time between current frame and last frame
float last_frame = 0.0F; // Time of last frame

// Define your cube positions again (or ensure the array is visible here)
std::array<glm::vec3, 10> cube_positions = {
    glm::vec3(0.0F, 0.0F, 0.0F),    glm::vec3(2.0F, 5.0F, -15.0F),
    glm::vec3(-1.5F, -2.2F, -2.5F), glm::vec3(-3.8F, -2.0F, -12.3F),
    glm::vec3(2.4F, -0.4F, -3.5F),  glm::vec3(-1.7F, 3.0F, -7.5F),
    glm::vec3(1.3F, -2.0F, -2.5F),  glm::vec3(1.5F, 2.0F, -2.5F),
    glm::vec3(1.5F, 0.2F, -1.5F),   glm::vec3(-1.3F, 1.0F, -1.5F)};

void FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window,
                             1); // TODO: Change this to show in game pause UI
  }

  const float CAMERA_SPEED =
      20.0F * delta_time; // Adjusted speed factor for better responsiveness
  // Forward (W)
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera->MoveForeward(CAMERA_SPEED);
  }

  // Backward (S)
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera->MoveBackward(CAMERA_SPEED);
  }

  // Strafe Left (A) - Moves perpendicular to the view
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera->MoveLeft(CAMERA_SPEED);
  }

  // Strafe Right (D) - Moves perpendicular to the view
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera->MoveRight(CAMERA_SPEED);
  }

  if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
}

float vertices[] = {
    -0.5F, -0.5F, -0.5F, 0.0F, 0.0F, 0.5F,  -0.5F, -0.5F, 1.0F, 0.0F,
    0.5F,  0.5F,  -0.5F, 1.0F, 1.0F, 0.5F,  0.5F,  -0.5F, 1.0F, 1.0F,
    -0.5F, 0.5F,  -0.5F, 0.0F, 1.0F, -0.5F, -0.5F, -0.5F, 0.0F, 0.0F,

    -0.5F, -0.5F, 0.5F,  0.0F, 0.0F, 0.5F,  -0.5F, 0.5F,  1.0F, 0.0F,
    0.5F,  0.5F,  0.5F,  1.0F, 1.0F, 0.5F,  0.5F,  0.5F,  1.0F, 1.0F,
    -0.5F, 0.5F,  0.5F,  0.0F, 1.0F, -0.5F, -0.5F, 0.5F,  0.0F, 0.0F,

    -0.5F, 0.5F,  0.5F,  1.0F, 0.0F, -0.5F, 0.5F,  -0.5F, 1.0F, 1.0F,
    -0.5F, -0.5F, -0.5F, 0.0F, 1.0F, -0.5F, -0.5F, -0.5F, 0.0F, 1.0F,
    -0.5F, -0.5F, 0.5F,  0.0F, 0.0F, -0.5F, 0.5F,  0.5F,  1.0F, 0.0F,

    0.5F,  0.5F,  0.5F,  1.0F, 0.0F, 0.5F,  0.5F,  -0.5F, 1.0F, 1.0F,
    0.5F,  -0.5F, -0.5F, 0.0F, 1.0F, 0.5F,  -0.5F, -0.5F, 0.0F, 1.0F,
    0.5F,  -0.5F, 0.5F,  0.0F, 0.0F, 0.5F,  0.5F,  0.5F,  1.0F, 0.0F,

    -0.5F, -0.5F, -0.5F, 0.0F, 1.0F, 0.5F,  -0.5F, -0.5F, 1.0F, 1.0F,
    0.5F,  -0.5F, 0.5F,  1.0F, 0.0F, 0.5F,  -0.5F, 0.5F,  1.0F, 0.0F,
    -0.5F, -0.5F, 0.5F,  0.0F, 0.0F, -0.5F, -0.5F, -0.5F, 0.0F, 1.0F,

    -0.5F, 0.5F,  -0.5F, 0.0F, 1.0F, 0.5F,  0.5F,  -0.5F, 1.0F, 1.0F,
    0.5F,  0.5F,  0.5F,  1.0F, 0.0F, 0.5F,  0.5F,  0.5F,  1.0F, 0.0F,
    -0.5F, 0.5F,  0.5F,  0.0F, 0.0F, -0.5F, 0.5F,  -0.5F, 0.0F, 1.0F};

unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

Window *main_window = nullptr;

VBO* vbo = nullptr;
VAO* vao = nullptr;
EBO* ebo = nullptr;
ShaderProgram *shader = nullptr;

void InitGLFW() {
  if (glfwInit() == 0) {
    std::cerr << "Failed to initialize GLFW" << '\n';
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 4);
}

void InitOpenGL() {
  if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
    Debug::Critical(MODULE_NAME, "Failed to initialize GLAD");
    return;
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glViewport(0, 0, WINDOW_PARAMS.size.w, WINDOW_PARAMS.size.h);
  glfwSetFramebufferSizeCallback(main_window->GetWindow(),
                                 FramebufferSizeCallback);
  glfwSwapInterval(1); // Enable vsync
}

int main(int argc, char **argv) {
  InitGLFW();
  Debug::Info(MODULE_NAME, "Setting up window");

  main_window =
      new Window(WINDOW_PARAMS.size, std::string(WINDOW_PARAMS.title));

  InitOpenGL();

  Debug::Info(MODULE_NAME, "Setting up graphics buffers");

  // Shader setup
  vbo = new VBO();
  vao = new VAO(vbo);
  ebo = new EBO();
  shader = new ShaderProgram();

  vao->Bind();
  vbo->Bind();
  VBO::SetBufferData(sizeof(vertices), vertices);
  ebo->Bind();
  EBO::SetBufferData(sizeof(indices), indices);

  // Location 0 (Position): index=0, size=3, stride_floats=5, offset_floats=0
  vao->SetBufferData(0, 3, 5, 0);
  // Location 2 (TexCoords): index=2, size=2, stride_floats=5, offset_floats=3
  vao->SetBufferData(2, 2, 5, 3);

  auto *tex = new Texture2D("gamedata/textures/testtex.png");

  if (!tex->Generate()) {
    Debug::Critical(MODULE_NAME, "Texture generation failed. Exiting.");
    return -1;
  }

  if (shader->GetId() == 0) {
    Debug::Critical(MODULE_NAME, "Invalid shader program found. Exiting.");
    return -1;
  }

  shader->Use();
  Texture2D::Activate(GL_TEXTURE0);
  tex->Bind();
  glUniform1i(glGetUniformLocation(shader->GetId(), "texture1"),
              0); // set it manually

  // After creating the window and before the main loop:
  glfwSetWindowUserPointer(main_window->GetWindow(), camera);
  glfwSetCursorPosCallback(main_window->GetWindow(), Camera::MouseCallback);

  // Capture and hide the cursor for FPS-style camera
  glfwSetInputMode(main_window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  Debug::Info(MODULE_NAME, "Starting main loop");

  model = glm::rotate(model, glm::radians(-55.0F), glm::vec3(1.0F, 0.0F, 0.0F));

  int model_loc = glGetUniformLocation(shader->GetId(), "model");
  int view_loc = glGetUniformLocation(shader->GetId(), "view");
  int proj_loc = glGetUniformLocation(shader->GetId(), "projection");

  glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj));

  // Main loop
  while (glfwWindowShouldClose(main_window->GetWindow()) == 0) {

    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    ProcessInput(main_window->GetWindow());

    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->Use();
    Texture2D::Activate(GL_TEXTURE0);
    tex->Bind();
    vao->Bind();

    glUniformMatrix4fv(view_loc, 1, GL_FALSE,
                       glm::value_ptr(camera->GetViewMatrix()));

    for (unsigned int i = 0; i < 10; i++) {
      auto current_model = glm::mat4(1.0F); // Start fresh for each cube
      auto pos = cube_positions.at(i);
      // Translate and Rotate the cube
      current_model = glm::translate(current_model, pos);
      float angle =
          20.0F * static_cast<float>(i); // Use the fixed rotation value
      current_model = glm::rotate(current_model, glm::radians(angle),
                                  glm::vec3(1.0F, 0.3F, 0.5F));

      // Upload the Model matrix
      glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(current_model));

      // Draw the cube
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    VAO::Unbind();

    glfwSwapBuffers(main_window->GetWindow());
    glfwPollEvents();
  }

  // Cleanup
  delete vbo;
  delete vao;
  delete ebo;
  delete shader;

  glfwDestroyWindow(main_window->GetWindow());

  glfwTerminate();

  return 0;
}
