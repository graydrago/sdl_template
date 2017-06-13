#pragma once

#include <glm/glm.hpp>

class Collider {
    private:
        glm::vec3 m_min;
        glm::vec3 m_max;

    public:
        Collider() : m_min(-1.f), m_max(1.f) {};
        virtual ~Collider() {};

        void setMin(const glm::vec3 &v) noexcept { m_min = v; }
        void setMax(const glm::vec3 &v) noexcept { m_max = v; }

        glm::vec3 getMin() const noexcept { return m_min; }
        glm::vec3 getMix() const noexcept { return m_max; }

        bool test(glm::vec3 point) const noexcept {
            return glm::all(glm::lessThanEqual(m_min, point)) &&
                   glm::all(glm::greaterThanEqual(m_max, point));
        }
};
