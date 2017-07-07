#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
        m_shaderProgram->setUniform("LightPosition", Game::instance().lightPosition());
        m_shaderProgram->setUniform("PVM", PVM);
        m_shaderProgram->setUniform("VM", wt);
        m_shaderProgram->setUniform("NM", NM);

        glBindVertexArray(m_mesh->VAO());
        glBindBuffer(GL_ARRAY_BUFFER, m_mesh->vertexBuffer());
        glDrawArrays(GL_TRIANGLES, 0, m_mesh->geometry().verticesIndeces().size());
    }
}

