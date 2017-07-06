#include <glm/gtc/matrix_transform.hpp>

#include "../headers/Camera.h"
#include <iostream>

Camera::Camera() :
    Object(),
    m_up(0.f, 1.f, 0.f),
    m_right(glm::cross(m_forward, m_up)),
    m_forward(0.f, 0.f, 0.f),
    m_yaw(-90),
    m_pitch(0),
    m_speed(0.02)
{
    setPosition(glm::vec3(0.f, 0.f, 2.f));
}


glm::mat4 Camera::eye() {
    if (m_pitch > 89) { m_pitch = 89; }
    else if (m_pitch < -89) { m_pitch = -89; }

    auto r_pitch = glm::radians(m_pitch);
    auto r_yaw = glm::radians(m_yaw);

    m_forward.x = glm::cos(r_pitch) * glm::cos(r_yaw);
    m_forward.y = glm::sin(-r_pitch);
    m_forward.z = glm::cos(r_pitch) * glm::sin(r_yaw);

    return glm::lookAt(getPosition(), getPosition() + glm::normalize(m_forward), m_up);
}

