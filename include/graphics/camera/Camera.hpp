#pragma once

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Camera {

    public:
    Camera();
    void LookAt(glm::mat4& view_matrix_out);

    [[nodiscard]] glm::vec3 GetCameraPos() const { return camera_pos; };
    [[nodiscard]] glm::vec3 GetCameraTarget() const { return camera_target; };
    [[nodiscard]] glm::vec3 GetCameraDirection() const { return camera_direction; };
    [[nodiscard]] glm::vec3 GetCameraUp() const { return camera_up; };
    [[nodiscard]] glm::vec3 GetCameraFront() const { return camera_front; };
    glm::mat4 GetViewMatrix();
    void SetCameraUp(glm::vec3 camera_up) { this->camera_up = camera_up; };
    void SetCameraFront(glm::vec3 camera_front) { this->camera_front = camera_front; };
    void SetCameraPos(glm::vec3 camera_pos) { this->camera_pos = camera_pos; };
    void SetCameraDirection(glm::vec3 camera_direction) { this->camera_direction = camera_direction; };
    void SetCameraTarget(glm::vec3 camera_target) { this->camera_target = camera_target; };

    void MoveForeward(float speed);
    void MoveBackward(float speed);
    void MoveLeft(float speed);
    void MoveRight(float speed);
    // void camera_mouse_callback(GLFWwindow* window, double xpos, double ypos);

    private:
        glm::vec3 camera_pos = glm::vec3(0.0F, 0.0F, 3.0F);
        glm::vec3 camera_target = glm::vec3(0.0F, 0.0F, 0.0F);
        glm::vec3 camera_front = glm::vec3(0.0F, 0.0F, -1.0F);
        glm::vec3 camera_direction = glm::normalize(camera_pos - camera_target);
        glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_direction));
        glm::vec3 camera_up = glm::cross(camera_direction, camera_right);

        glm::mat4 view = glm::mat4(1.0F);
        glm::vec3 up = glm::vec3(0.0F, 1.0F, 0.0F);
        static float yaw;
        static float pitch;
        static float lastX, lastY;
        static bool firstMouse;
};
