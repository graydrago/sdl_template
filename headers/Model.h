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
        glm::vec3 m_color;
        std::shared_ptr<Mesh> m_mesh;
        std::shared_ptr<ShaderProgram> m_shaderProgram;
        std::vector<std::shared_ptr<Model>> children;

    public:
        Model();
        virtual ~Model() {};

        //void addChild(std::shared_ptr<Model> model) noexcept { children.push_back(std::move(model)); }
        void setMesh(std::shared_ptr<Mesh> v) noexcept { m_mesh = v; }
        void setShader(std::shared_ptr<ShaderProgram> v) noexcept { m_shaderProgram = v; }
        //void setColor(const glm::vec3& v) noexcept { m_color = v; }

        //glm::vec3 getColor() const noexcept { return m_color; }
        void render(const glm::mat4 &P, const glm::mat4 &V) const noexcept;
        //void render(const glm::mat4 &P) const noexcept;
};
