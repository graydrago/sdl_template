#include <glm/gtc/matrix_transform.hpp>

#include "../headers/Camera.h"
#include <iostream>

Camera::Camera() {
    up = {0.f, 1.f, 0.f};
    forward = {0.f, 0.f, 0.f};
    right = glm::cross(forward, up);
    position = {0.f, 0.f, 2.f};
    update_cb = nullptr;
    yaw = -90;
    pitch = 0;
}


glm::mat4 Camera::eye() {
    if (pitch > 89) { pitch = 89; }
    else if (pitch < -89) { pitch = -89; }

    auto r_pitch = glm::radians(pitch);
    auto r_yaw = glm::radians(yaw);

    forward.x = glm::cos(r_pitch) * glm::cos(r_yaw);
    forward.y = glm::sin(-r_pitch);
    forward.z =  glm::cos(r_pitch) * glm::sin(r_yaw);

    return glm::lookAt(position, position + forward, up);
}


void Camera::setUpdateCb(std::function<void(Camera&, float)> cb) {
    update_cb = cb;
}


void Camera::update(float elapsed_time) {
    update_cb(*this, elapsed_time);
}
