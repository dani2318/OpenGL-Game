#include <array>
#include <iostream>
#include <string>
#include <vector>

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
#include <graphics/textures/TextureAtlas.hpp>
#include <graphics/window.hpp>
#include <graphics/lighting/light.hpp>
#include <graphics/shapes/Cubes.hpp>


#include <utils/debug.hpp>
#define MODULE_NAME "Main"

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


Window *main_window = nullptr;

ShaderProgram *shader = nullptr;

const int x_size = 16;
const int y_size = 16;
const int z_size = 16;
std::array<glm::vec3, x_size*y_size*z_size> cube_positions = {};

// Helper function to check if a block exists at position
bool IsBlockSolid(int x, int y, int z) {
    if (x < 0 || x >= x_size || y < 0 || y >= y_size || z < 0 || z >= z_size) {
        return false; // Outside bounds = no block
    }
    return true; // For now, all positions have blocks
}

// Calculate which faces should be visible
uint8_t CalculateVisibleFaces(int x, int y, int z) {
    uint8_t faces = 0;

    // Check each direction
    if (!IsBlockSolid(x, y, z + 1)) faces |= (uint8_t)CubeFace::FRONT;
    if (!IsBlockSolid(x, y, z - 1)) faces |= (uint8_t)CubeFace::BACK;
    if (!IsBlockSolid(x - 1, y, z)) faces |= (uint8_t)CubeFace::LEFT;
    if (!IsBlockSolid(x + 1, y, z)) faces |= (uint8_t)CubeFace::RIGHT;
    if (!IsBlockSolid(x, y + 1, z)) faces |= (uint8_t)CubeFace::TOP;
    if (!IsBlockSolid(x, y - 1, z)) faces |= (uint8_t)CubeFace::BOTTOM;

    return faces;
}

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

  // // Enable backface culling
  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);           // Cull back faces
  // glFrontFace(GL_CCW);           // Front faces are counter-clockwise

  // Cull front faces instead
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);      // Changed from GL_BACK
  glFrontFace(GL_CW);

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
  Light* main_lighting = new Light(camera);

  shader = new ShaderProgram();

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

  auto* atlas = new TextureAtlas();

  TextureAtlasElement element = {};

  Texture2D* tex = new Texture2D("gamedata/textures/testtex.png");

  if (!tex->Generate()) {
    Debug::Critical(MODULE_NAME, "Texture generation failed. Exiting.");
  }

  atlas->AddTexture(tex);
  std::vector<Cube*> cubes;
  for(int y = 0; y < y_size; y++) {
      for(int x = 0; x < x_size; x++) {
          for(int z = 0; z < z_size; z++) {
              int index = y * x_size * z_size + x * z_size + z;
              cube_positions[index] = glm::vec3(x, y, z);

              uint8_t visible_faces = CalculateVisibleFaces(x, y, z);
              cubes.push_back(new Cube(shader, cube_positions[index],
                                      atlas->GetElementByID(0)->texture,
                                      main_lighting, camera, visible_faces));
          }
      }
  }
  Debug::Info(MODULE_NAME, "Atlas texture count %d", atlas->GetCount());

  // Main loop
  while (glfwWindowShouldClose(main_window->GetWindow()) == 0) {

    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    ProcessInput(main_window->GetWindow());

    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->Use();
    glUniform3fv(glGetUniformLocation(shader->GetId(), "lightPos"), 1,
                 glm::value_ptr(main_lighting->GetPos()));

    for (Cube* cube : cubes) {
        cube->Draw(view_loc, model_loc);
    }

    // Sun arc from horizon to horizon
    static float sun_angle = 0.0f;
    sun_angle += 0.2f * delta_time;

    float orbit_radius = 60.0f;
    glm::vec3 center = cube_positions.at(0);

    main_lighting->SetPos({center.x + (orbit_radius * cos(sun_angle)), center.y + (orbit_radius * sin(sun_angle)),  center.z});

    main_lighting->PaintLight(camera->GetViewMatrix());

    glfwSwapBuffers(main_window->GetWindow());
    glfwPollEvents();
  }

  // Cleanup
  delete shader;

  glfwDestroyWindow(main_window->GetWindow());

  glfwTerminate();

  return 0;
}
