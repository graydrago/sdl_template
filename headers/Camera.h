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

    public:
        Camera();
        virtual ~Camera() {};
        glm::mat4 eye();

        void setYaw(float v) noexcept { m_yaw = v; };
        void setPitch(float v) noexcept { m_pitch = v; };
        void setUp(glm::vec3 v) noexcept { m_up = v; }
        void setRight(glm::vec3 v) noexcept { m_right = v; }
        void setForward(glm::vec3 v) noexcept { m_forward = v; }

        float getYaw() const noexcept { return m_yaw; };
        float getPitch() const noexcept { return m_pitch; };
        glm::vec3 getUp() const noexcept { return m_up; }
        glm::vec3 getRight() const noexcept { return m_right; }
        glm::vec3 getForward() const noexcept { return m_forward; }
};
