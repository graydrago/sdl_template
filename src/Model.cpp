#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../headers/Model.h"
#include "../headers/utils.h"


Model::Model() {
    m_color = glm::vec3(0.f, 0.f, 1.f);
    matrix = glm::mat4(1.f);
    update_cb = nullptr;
}


Model::~Model() {}


void Model::render(const glm::mat4 &V, const glm::mat4 &P) {
    auto VM = V * matrix;
    if (m_mesh && m_shaderProgram) {
        auto PVM = P * VM;
        auto NM = glm::mat3(glm::transpose(glm::inverse(VM)));

        m_shaderProgram->use();
        m_shaderProgram->setUniform("Color", m_color);
        m_shaderProgram->setUniform("PVM", PVM);
        m_shaderProgram->setUniform("VM", VM);
        m_shaderProgram->setUniform("NM", NM);

        glBindVertexArray(m_mesh->vaoHandle());
        glBindBuffer(GL_ARRAY_BUFFER, m_mesh->vertexBuffer());
        glDrawArrays(GL_TRIANGLES, 0, m_mesh->geometry().verticesIndeces().size());
    }

    for (auto& child : children) {
        child->render(VM, P);
    }
}


void Model::attachShader(std::shared_ptr<ShaderProgram> shader) {
    m_shaderProgram = shader;
}


void Model::setMesh(std::shared_ptr<Mesh> mesh) {
    m_mesh = mesh;
}


void Model::addChild(std::shared_ptr<Model> model) {
    children.push_back(std::move(model));
}


void Model::color(const glm::vec3& _color) noexcept {
    m_color = _color;
}


void Model::setUpdateCb(std::function<void(Model&, float)> cb) {
    update_cb = cb;
}

void Model::update(float elapsed_time) {
    if (update_cb) {
        update_cb(*this, elapsed_time);
    }

    for (auto& child : children) {
        child->update(elapsed_time);
    }
}
