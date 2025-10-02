#include "Camera.hpp"

Camera::Camera(){
    // this -> firstMouse = true;
    // this -> yaw = -90.0F;
    // this -> pitch = 90.F;
}

void Camera::MoveForeward(float speed){
    this->camera_pos += speed * camera_front;
}


void Camera::MoveBackward(float speed){
    this->camera_pos -= speed * camera_front;
}

void Camera::MoveLeft(float speed){
    this->camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * speed;
}

void Camera::MoveRight(float speed){
    this->camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * speed;
}

glm::mat4 Camera::GetViewMatrix(){
    return glm::lookAt(this->camera_pos,
                       this->camera_pos + this->camera_front,
                       this->camera_up);
}

/*
void Camera::camera_mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front = glm::normalize(direction);
}
 */
