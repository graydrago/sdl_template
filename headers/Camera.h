#pragma once

#include <glm/glm.hpp>
#include <functional>

#include "Object.h"

class Camera : public Object {
    private:
        glm::vec3 m_up;
        glm::vec3 m_right;
        glm::vec3 m_forward;
        float m_yaw;
        float m_pitch;
        float m_speed;

    public:
        Camera();
        virtual ~Camera() {};
        glm::mat4 eye();

        const Camera& yaw(float v) noexcept { m_yaw = v; return *this; }
        const Camera& pitch(float v) noexcept { m_pitch = v; return *this; }
        const Camera& up(glm::vec3 v) noexcept { m_up = v; return *this; }
        const Camera& right(glm::vec3 v) noexcept { m_right = v; return *this; }
        const Camera& forward(glm::vec3 v) noexcept { m_forward = v; return *this; }
        const Camera& speed(float v) noexcept { m_speed = v; return *this; }

        float yaw() const noexcept { return m_yaw; }
        float pitch() const noexcept { return m_pitch; }
        glm::vec3 up() const noexcept { return m_up; }
        glm::vec3 right() const noexcept { return m_right; }
        glm::vec3 forward() const noexcept { return m_forward; }
        float speed() noexcept { return m_speed; }
};
