#pragma once
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
#include <graphics/window.hpp>
#include <graphics/lighting/light.hpp>
#include <graphics/shapes/Cubes.hpp>

enum class BlockType {
    AIR = 0,
    SOLID = 1,
    // Add more types later (GRASS, STONE, etc.)
};
inline bool IsBlockSolid(BlockType type) {
    return type != BlockType::AIR;
}

enum class CubeFace {
    FRONT  = 0b000001,  // +Z
    BACK   = 0b000010,  // -Z
    LEFT   = 0b000100,  // -X
    RIGHT  = 0b001000,  // +X
    TOP    = 0b010000,  // +Y
    BOTTOM = 0b100000   // -Y
};

class Cube{

    public:
        Cube(ShaderProgram* main_shader, glm::vec3 pos, Texture2D *tex, Light* main_light, Camera* main_camera, uint8_t visible_faces);
        bool Draw(int view_loc, int model_loc);
        void SetVisibleFaces(uint8_t faces) { visible_faces = faces; needs_remesh = true; }
        void UpdateMesh();
    private:
        void GenerateMesh();

        ShaderProgram* shader = nullptr;
        static float vertices[];
        Camera* main_camera = nullptr;
        static unsigned int indices[];
        Light* main_light = nullptr;
        VAO* cube_vao = nullptr;
        EBO* cube_ebo = nullptr;
        VBO* cube_vbo = nullptr;
        glm::vec3 pos;
        Texture2D *tex = nullptr;

        uint8_t visible_faces; // Bitfield for which faces to render
        bool needs_remesh = true;
        std::vector<float> mesh_vertices;

};
