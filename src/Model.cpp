#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../headers/Camera.h"
#include "../headers/Game.h"
#include "../headers/Model.h"
#include "../headers/Mesh.h"
#include "../headers/utils.h"


void Model::render(const glm::mat4 &P, const glm::mat4 &V) const noexcept {
    if (m_mesh && m_shaderProgram) {
        auto wt = worldTransform();
        auto PVM = P * V * wt;
        auto NM = glm::mat3(glm::transpose(glm::inverse(wt)));

        m_shaderProgram->use();
        m_shaderProgram->setUniform("Color", color());
        m_shaderProgram->setUniform("light.position", Game::instance().light()->position());
        m_shaderProgram->setUniform("light.ambient", Game::instance().light()->ambient());
        m_shaderProgram->setUniform("light.diffuse", Game::instance().light()->diffuse());
        m_shaderProgram->setUniform("light.specular", Game::instance().light()->specular());
        m_shaderProgram->setUniform("material.ambient", ambient());
        m_shaderProgram->setUniform("material.diffuse", diffuse());
        m_shaderProgram->setUniform("material.specular", specular());
        m_shaderProgram->setUniform("material.shininess", shininess());
        m_shaderProgram->setUniform("CameraPosition", Game::instance().camera()->position());
        m_shaderProgram->setUniform("PVM", PVM);
        m_shaderProgram->setUniform("VM", wt);
        m_shaderProgram->setUniform("NM", NM);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_mesh->vertexBuffer());
        glDrawArrays(GL_TRIANGLES, 0, m_mesh->geometry().indices().size());
    }
}


void Model::mesh(std::shared_ptr<Mesh> v) noexcept {
    m_mesh = v;

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_mesh->vertexBuffer());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    if (m_mesh->normalBuffer() > 0) {
      glBindBuffer(GL_ARRAY_BUFFER, m_mesh->normalBuffer());
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
      glEnableVertexAttribArray(1);
      gl_check_error();
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
