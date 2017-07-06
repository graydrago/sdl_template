#pragma once

#include <memory>
#include <functional>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Collider.h"
#include "utils.h"
#include "SphereCollider.h"

class Object {
    private:
        std::function<void(Object&, float)> m_updateCb;
        std::function<void(Object&, float)> m_renderCb;
        std::shared_ptr<Collider> m_collider;
        glm::vec3 m_color;
        glm::vec3 m_scale;
        glm::vec3 m_rotation;
        glm::vec3 m_position;

    public:

        Object() noexcept;
        virtual ~Object() {};
        virtual void update(float elapsed_time) noexcept;
        virtual void render(const glm::mat4 &, const glm::mat4 &) const noexcept {};

        virtual void updateCb(std::function<void(Object&, float)> _cb) noexcept { m_updateCb = _cb; }
        virtual void renderCb(std::function<void(Object&, float)> _cb) noexcept { m_renderCb = _cb; }
        virtual void color(const glm::vec3 &v) noexcept { m_color = v; }
        virtual void position(const glm::vec3 &v) noexcept { m_position = v; }
        virtual void scale(const glm::vec3 &v) noexcept { m_scale = v; }
        virtual void rotation(const glm::vec3 &v) noexcept { m_rotation = v; }
        virtual void collider(std::shared_ptr<Collider> v) noexcept { m_collider = v; }

        virtual glm::vec3 color() const noexcept { return m_color; }
        virtual glm::vec3 position() const noexcept { return m_position; }
        virtual glm::vec3 scale() const noexcept { return m_scale; }
        virtual glm::vec3 rotation() const noexcept { return m_rotation; }
        std::shared_ptr<Collider> collider() noexcept { return m_collider; }

        virtual glm::mat4 worldTransform() const noexcept;
};
