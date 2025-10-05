#pragma once

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


class Cube{

    public:
        Cube(ShaderProgram* main_shader, glm::vec3 pos, Texture2D *tex, Light* main_light, Camera* main_camera);
        bool Draw(int view_loc, int model_loc);

    private:
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
};
