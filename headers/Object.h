#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <glm/glm.hpp>

class Object {
    private:
        std::function<void(Object&, float)> m_updateCb;
        std::function<void(Object&, float)> m_renderCb;
        glm::vec3 m_color;
        glm::vec3 m_position;
        glm::vec3 m_rotation;
        glm::mat4 m_matrix;
        glm::mat4 m_viewMatrix;
        std::vector<std::shared_ptr<Object>> m_children;

    public:

        Object() noexcept;
        virtual ~Object() {};
        void update(float elapsed_time, const glm::mat4 &V) noexcept;
        void render(const glm::mat4 &P) noexcept;

        void setUpdateCb(std::function<void(Object&, float)> _cb) noexcept { m_updateCb = _cb; }
        void setRenderCb(std::function<void(Object&, float)> _cb) noexcept { m_renderCb = _cb; }
        void setColor(const glm::vec3 &_v) noexcept { m_color = _v; }
        void setPosition(const glm::vec3 &_v) noexcept { m_position = _v; }
        void setRotation(const glm::vec3 &_v) noexcept { m_rotation = _v; }
        void setMatrix(const glm::mat4 &_v) noexcept { m_matrix = _v; }
        void addChild(std::shared_ptr<Object> obj) noexcept { m_children.push_back(obj); }

        glm::vec3 getColor() const noexcept { return m_color; }
        glm::vec3 getPosition() const noexcept { return m_position; }
        glm::vec3 getRotation() const noexcept { return m_rotation; }
        glm::mat4 getMatrix() const noexcept { return m_matrix; }
};
