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

        // for diffuse is used m_color
        glm::vec3 m_ambient;
        glm::vec3 m_specular;
        float m_shininess;

        GLuint m_vao;

    public:
        Model() : m_ambient({0.1, 0.1, 0.1}),
                  m_specular({0.1, 0.1, 0.1}),
                  m_shininess(100) {};

        virtual ~Model() { glDeleteVertexArrays(1, &m_vao); };

        virtual void mesh(std::shared_ptr<Mesh> v) noexcept;
        virtual void shader(std::shared_ptr<ShaderProgram> v) noexcept { m_shaderProgram = v; }

        virtual const std::shared_ptr<Mesh> mesh() const noexcept { return m_mesh; }
        virtual const std::shared_ptr<ShaderProgram> shader() const noexcept { return m_shaderProgram; }
        GLuint vao() const noexcept { return m_vao; }

        void render(const glm::mat4 &P, const glm::mat4 &V) const noexcept;

        virtual void ambient(const glm::vec3 &v) noexcept { m_ambient = v; }
        virtual glm::vec3 ambient() const noexcept { return m_ambient; }

        virtual void diffuse(const glm::vec3 &v) noexcept { color(v); }
        virtual glm::vec3 diffuse() const noexcept { return color(); }

        virtual void specular(const glm::vec3 &v) noexcept { m_specular = v; }
        virtual glm::vec3 specular() const noexcept { return m_specular; }

        virtual void shininess(float v) noexcept { m_shininess = v; }
        virtual float shininess() const noexcept { return m_shininess; }
};
