// Camera.cpp
#include "Camera.hpp"

#include <algorithm>

// Initialize static members
float Camera::yaw = -90.0F;
float Camera::pitch = 0.0F;
float Camera::last_x = 640.0F;  // Half of your window width
float Camera::last_y = 360.0F;  // Half of your window height
bool Camera::first_mouse = true;

Camera::Camera() {
    // Initialize camera vectors
    camera_direction = glm::normalize(camera_pos - camera_target);
    camera_right = glm::normalize(glm::cross(up, camera_direction));
    camera_up = glm::cross(camera_direction, camera_right);
}

void Camera::LookAt(glm::mat4& view_matrix_out) {
    view_matrix_out = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
}

void Camera::MoveForeward(float speed) {
    camera_pos += speed * camera_front;
}

void Camera::MoveBackward(float speed) {
    camera_pos -= speed * camera_front;
}

void Camera::MoveLeft(float speed) {
    camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * speed;
}

void Camera::MoveRight(float speed) {
    camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * speed;
}

void Camera::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    auto* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (cam != nullptr) {
        cam->ProcessMouseMovement(xpos, ypos);
    }
}

void Camera::ProcessMouseMovement(double xpos, double ypos) {
    if (first_mouse) {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos; // Reversed since y-coordinates range from bottom to top
    last_x = xpos;
    last_y = ypos;

    const float SENSITIVITY = 0.1F;
    xoffset *= SENSITIVITY;
    yoffset *= SENSITIVITY;

    yaw += xoffset;
    pitch += yoffset;

    // Constrain pitch to prevent screen flip
    pitch = std::min(pitch, 89.0f);
    pitch = std::max(pitch, -89.0f);

    // Update camera front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front = glm::normalize(front);

    // Also update right and up vectors
    camera_right = glm::normalize(glm::cross(camera_front, up));
    camera_up = glm::normalize(glm::cross(camera_right, camera_front));
}
