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
    has_loaded_movel = false;
    update_cb = nullptr;
}


Model::~Model() {}


void Model::load(std::string geometryFileName) {
    using std::cout;
    using std::endl;

    std::vector<float> vertices;
    std::vector<float> normals;

    geometry.load(geometryFileName);

    for (auto item : geometry.verticesIndeces()) {
        vertices.push_back(geometry.vertices()[item * 3 ]);
        vertices.push_back(geometry.vertices()[item * 3 + 1]);
        vertices.push_back(geometry.vertices()[item * 3 + 2]);
    }
    for (auto item : geometry.normalsIndeces()) {
        normals.push_back(geometry.normals()[item * 3 ]);
        normals.push_back(geometry.normals()[item * 3 + 1]);
        normals.push_back(geometry.normals()[item * 3 + 2]);
    }

    glGenBuffers(1, &vertexBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,
            vertices.size() * sizeof(float),
            &(vertices)[0],
            GL_STATIC_DRAW);

    glGenBuffers(1, &normalBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,
            normals.size() * sizeof(float),
            &(normals)[0],
            GL_STATIC_DRAW);

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    cout << vertices.size() << endl;
    cout << normals.size() << endl;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    gl_check_error();
    has_loaded_movel = true;
}

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


void Model::addChild(std::unique_ptr<Model> model) {
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
