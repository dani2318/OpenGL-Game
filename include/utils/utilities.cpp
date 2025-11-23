#include "utilities.hpp"
#include <iostream>

#include <utils/debug.hpp>


#define MODULE_NAME "Utils"

using namespace std;



filesystem::path GetExecutablePath() {
#ifdef _WIN32
    Debug::Info(MODULE_NAME, "Running under windows. Running search with windows API");
    vector<char> path(MAX_PATH);
    DWORD result = 0;

    while (true) {
        result = GetModuleFileNameA(nullptr, path.data(), static_cast<DWORD>(path.size()));

        if (result == 0) {
            throw runtime_error(std::format("Failed to get executable path, error: {}", GetLastError()));
        }

        if (result < path.size()) {
            break;
        }

        path.resize(path.size() * 2);
    }
#else
    Debug::Info(MODULE_NAME, "Running under Linux or macOS. Running search with Unix API");
    vector<char> path(PATH_MAX);
    ssize_t count = readlink("/proc/self/exe", path.data(), path.size());

    if (count == -1) {
        throw runtime_error(format("Failed to get executable path, errno: {}", errno));
    }

    if (count >= static_cast<ssize_t>(path.size())) {
        throw runtime_error("Executable path too long");
    }

    // readlink doesn't null-terminate, so we need to do it ourselves
    path[count] = '\0';
#endif
    return filesystem::path(path.data()).parent_path();
}


// Helper function to check if a block exists at position
bool IsBlockSolid(glm::vec3 position) {
  if (position.x < 0 || position.x >= x_size || position.y < 0 || position.y >= y_size || position.z < 0 || position.z >= z_size) {
    return false; // Outside bounds = no block
  }
  return true; // For now, all positions have blocks
}

uint8_t CheckFace(glm::vec3 position, CubeFace direction){
    uint8_t faces = 0;
    if (!IsBlockSolid({position.x, position.y, position.z}))
      return faces |= (uint8_t)direction;

    return faces;
}
// Calculate which faces should be visible
uint8_t CalculateVisibleFaces(int x, int y, int z) {
  // Check each direction
  return  CheckFace({x, y, z + 1}, CubeFace::FRONT) |
          CheckFace({x, y, z - 1}, CubeFace::BACK)  |
          CheckFace({x - 1, y, z}, CubeFace::LEFT)  |
          CheckFace({x + 1, y, z}, CubeFace::RIGHT) |
          CheckFace({x, y + 1, z}, CubeFace::TOP)   |
          CheckFace({x, y - 1, z}, CubeFace::BOTTOM);
}
