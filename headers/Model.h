#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <functional>

#include "ShaderProgram.h"
#include "Mesh.h"
#include "Object.h"

class Model : public Object {
    private:
        std::shared_ptr<Mesh> m_mesh;
        std::shared_ptr<ShaderProgram> m_shaderProgram;

    public:
        Model() {};
        virtual ~Model() {};

        void setMesh(std::shared_ptr<Mesh> v) noexcept { m_mesh = v; }
        const std::shared_ptr<Mesh> getMesh() const noexcept { return m_mesh; }

        void setShader(std::shared_ptr<ShaderProgram> v) noexcept { m_shaderProgram = v; }
        const std::shared_ptr<ShaderProgram> getShader() const noexcept { return m_shaderProgram; }

        void render(const glm::mat4 &P, const glm::mat4 &V) const noexcept;
};
