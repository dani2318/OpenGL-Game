#pragma once

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <graphics/buffers/VBO.hpp>
#include <graphics/buffers/VAO.hpp>
#include <graphics/buffers/EBO.hpp>
#include <graphics/Shader.hpp>

class Light {

    public:
        Light();
        void PaintLight(glm::mat4 cameraView);
        static glm::vec3 CalculateColor() { return lightColor * toyColor;};
        static glm::vec3 GetPos() {return lightPos;};
        static void SetPos(glm::vec3 newPos = {5.0f, 100.0f, 20.0f}) {lightPos = newPos;};
    private:
        static inline glm::vec3 coral{1.0f, 0.5f, 0.31f};
        static inline glm::vec3 lightColor{1.0f, 1.0f, 1.0f};
        static inline glm::vec3 toyColor{1.0f, 0.5f, 0.31f};
        ShaderProgram* lightShader = nullptr;
        VBO* lightVBO = new VBO();
        VAO* lightVAO = new VAO(lightVBO);
        EBO* lightEBO = new EBO();
        glm::mat4 proj = glm::perspective(
            glm::radians(45.0F), float(1280) / 720,
            0.1F, 100.0F);
        glm::mat4 model = glm::mat4(1.0f);
        static inline glm::vec3 lightPos{5.0f, 100.0f, 20.0f};
};
