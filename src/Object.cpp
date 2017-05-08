#include "../headers/Object.h"

Object::Object() noexcept :
    m_color(0.f, 0.f, 0.f),
    m_position(0.f, 0.f, 0.f),
    m_rotation(0.f, 0.f, 0.f),
    m_matrix(1.f),
    m_viewMatrix(1.f),
    m_children()
{}


void Object::update(float elapsed_time, const glm::mat4 &V) noexcept {
    m_viewMatrix = V * m_matrix;

    if (m_updateCb) {
        m_updateCb(*this, elapsed_time);
    }

    for (auto& child : m_children) {
        child->update(elapsed_time, m_viewMatrix);
    }
};


void Object::render(const glm::mat4 &P) noexcept {
    for (auto& child : m_children) {
        child->render(P);
    }
}
