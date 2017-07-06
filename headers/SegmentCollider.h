#pragma once

#include <glm/glm.hpp>

#include "./Collider.h"
#include "./SphereCollider.h"

class SegmentCollider : public Collider {
        glm::vec3 m_start_point;
        glm::vec3 m_end_point;
        
    public:
        SegmentCollider() :
            m_start_point(0.f, 0.f, 0.f),
            m_end_point(0.f, 0.f, 1.f) {};
        SegmentCollider(glm::vec3 start_point, glm::vec3 end_point) :
            m_start_point(start_point),
            m_end_point(end_point) {};

        const SegmentCollider& startPoint(glm::vec3 v) { m_start_point = v; return *this; }
        const SegmentCollider& endPoint(glm::vec3 v) { m_start_point = v; return *this; }
        glm::vec3 startPoint() const { return m_start_point; }
        glm::vec3 endPoint() const { return m_end_point; }
};
