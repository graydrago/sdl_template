#pragma once

#include <memory>

#include "Object.h"
#include "ShaderProgram.h"

class Mesh;

class Model : public Object {
    private:
        std::shared_ptr<Mesh> m_mesh;
        std::shared_ptr<ShaderProgram> m_shaderProgram;

    public:
        Model() {};
        virtual ~Model() {};

        void mesh(std::shared_ptr<Mesh> v) noexcept { m_mesh = v; }
        void shader(std::shared_ptr<ShaderProgram> v) noexcept { m_shaderProgram = v; }

        const std::shared_ptr<Mesh> mesh() const noexcept { return m_mesh; }
        const std::shared_ptr<ShaderProgram> shader() const noexcept { return m_shaderProgram; }

        void render(const glm::mat4 &P, const glm::mat4 &V) const noexcept;
};
