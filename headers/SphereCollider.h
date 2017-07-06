#pragma once

#include <glm/glm.hpp>

#include "./Collider.h"
#include "./SegmentCollider.h"

class SphereCollider : public Collider {
    private:
        glm::vec3 m_center;
        float m_radius;
        
    public:
        SphereCollider() :
            m_center(0.f, 0.f, 0.f),
            m_radius(1.f) {};
        SphereCollider(glm::vec3 center, float radius) :
            m_center(center),
            m_radius(radius) {};

        const SphereCollider& center(glm::vec3 v) { m_center = v; return *this; }
        const SphereCollider& radius(float v) { m_radius = v; return *this; }
        glm::vec3 center() const { return m_center; }
        float radius() const { return m_radius; }
};
