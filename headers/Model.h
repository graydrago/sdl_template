#pragma once

#include <memory>
#include <GL/glew.h>

#include "Object.h"
#include "ShaderProgram.h"

class Mesh;

class Model : public Object {
    private:
        std::shared_ptr<Mesh> m_mesh;
        std::shared_ptr<ShaderProgram> m_shaderProgram;

        GLuint m_vao;

    public:
        Model() {};
        virtual ~Model() { glDeleteVertexArrays(1, &m_vao); };

        virtual void mesh(std::shared_ptr<Mesh> v) noexcept;
        virtual void shader(std::shared_ptr<ShaderProgram> v) noexcept { m_shaderProgram = v; }

        virtual const std::shared_ptr<Mesh> mesh() const noexcept { return m_mesh; }
        virtual const std::shared_ptr<ShaderProgram> shader() const noexcept { return m_shaderProgram; }
        GLuint vao() const noexcept { return m_vao; }

        void render(const glm::mat4 &P, const glm::mat4 &V) const noexcept;
};
