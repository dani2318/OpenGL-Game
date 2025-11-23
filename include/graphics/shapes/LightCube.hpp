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


class LightCube{

    public:
        LightCube(ShaderProgram* main_shader, Camera* main_camera);
        bool Draw(glm::vec3 pos, int view_loc, int model_loc);

    private:
        ShaderProgram* shader = nullptr;
        static float vertices[];
        Camera* main_camera = nullptr;
        static unsigned int indices[];
        VAO* cube_vao = nullptr;
        EBO* cube_ebo = nullptr;
        VBO* cube_vbo = nullptr;
        Texture2D *tex = new Texture2D("gamedata/textures/testtex.png");
};
