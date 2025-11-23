#pragma once
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
#include <graphics/lighting/light.hpp>
#include <graphics/shapes/Cubes.hpp>
#include <graphics/textures/Texture.hpp>
#include <graphics/textures/TextureAtlas.hpp>
#include <graphics/window.hpp>
#include <utils/utilities.hpp>

#include <utils/debug.hpp>
#define MODULE_NAME "Main"

constexpr WindowParameters WINDOW_PARAMS = {.size = {.w = 1920, .h = 1280},
                                            .title = "OpenGL game"};
