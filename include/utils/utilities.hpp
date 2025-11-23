#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <graphics/shapes/Cubes.hpp>

std::filesystem::path GetExecutablePath();

constexpr int x_size = 16;
constexpr int y_size = 16;
constexpr int z_size = 16;

bool IsBlockSolid(glm::vec3 position);
uint8_t CheckFace(glm::vec3 position, CubeFace direction);
inline std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Could not open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
uint8_t CalculateVisibleFaces(int x, int y, int z);
