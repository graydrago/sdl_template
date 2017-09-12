#pragma once

#include "Object.h"

class Light : public Object {
    private:
        glm::vec3 m_ambient;
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;

    public:
        Light() : m_ambient({0.1, 0.1, 0.1}),
                  m_diffuse({1, 1, 1}),
                  m_specular({1, 1, 1}) {};

        virtual void ambient(const glm::vec3 &v) noexcept { m_ambient = v; }
        virtual glm::vec3 ambient() const noexcept { return m_ambient; }

        virtual void diffuse(const glm::vec3 &v) noexcept { m_diffuse = v; }
        virtual glm::vec3 diffuse() const noexcept { return m_diffuse; }

        virtual void specular(const glm::vec3 &v) noexcept { m_specular = v; }
        virtual glm::vec3 specular() const noexcept { return m_specular; }
};
