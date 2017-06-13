#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <glm/glm.hpp>

#include "Collider.h"

class Object {
    private:
        std::function<void(Object&, float)> m_updateCb;
        std::function<void(Object&, float)> m_renderCb;
        glm::vec3 m_color;
        glm::vec3 m_scale;
        glm::vec3 m_rotation;
        glm::vec3 m_position;
        //glm::mat4 m_matrix;
        //glm::mat4 m_viewMatrix;
        //std::vector<std::shared_ptr<Object>> m_children;
        //std::shared_ptr<Collider> m_collider;

    public:

        Object() noexcept;
        virtual ~Object() {};
        //virtual void update(float elapsed_time, const glm::mat4 &V) noexcept = 0;
        virtual void update(float elapsed_time) noexcept;
        virtual void render(const glm::mat4 &, const glm::mat4 &) const noexcept {};
        //virtual void render(const glm::mat4 &) const noexcept {};

        virtual void setUpdateCb(std::function<void(Object&, float)> _cb) noexcept { m_updateCb = _cb; }
        virtual void setRenderCb(std::function<void(Object&, float)> _cb) noexcept { m_renderCb = _cb; }
        virtual void setColor(const glm::vec3 &v) noexcept { m_color = v; }
        virtual void setPosition(const glm::vec3 &v) noexcept { m_position = v; }
        virtual void setScale(const glm::vec3 &v) noexcept { m_scale = v; }
        virtual void setRotation(const glm::vec3 &v) noexcept { m_rotation = v; }
        //void setMatrix(const glm::mat4 &v) noexcept { m_matrix = v; }
        //void addChild(std::shared_ptr<Object> obj) noexcept { m_children.push_back(obj); }
        //void setCollider(std::shared_ptr<Collider> v) noexcept { m_collider = v; }

        virtual glm::vec3 getColor() const noexcept { return m_color; }
        virtual glm::vec3 getPosition() const noexcept { return m_position; }
        virtual glm::vec3 getScale() const noexcept { return m_scale; }
        virtual glm::vec3 getRotation() const noexcept { return m_rotation; }
        //virtual glm::mat4 getMatrix() const noexcept { return m_matrix; }
        //glm::mat4 getViewMatrix() const noexcept { return m_viewMatrix; }
        //std::shared_ptr<Collider> getCollider() const noexcept { return m_collider; }

        virtual glm::mat4 getWorldTransform() const noexcept;
};
