#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../headers/Game.h"
#include "../../headers/custom/TrianglePicker.h"
#include "../../headers/Mesh.h"


void TrianglePicker::render(const glm::mat4 &P, const glm::mat4 &V) const noexcept {
    auto mesh = this->mesh();
    auto shader = this->shader();
    if (mesh && shader) {
        auto wt = worldTransform();
        auto PVM = P * V * wt;
        auto NM = glm::mat3(glm::transpose(glm::inverse(wt)));

        shader->use();
        shader->setUniform("PVM", PVM);
        shader->setUniform("LightPosition", Game::instance().lightPosition());
        shader->setUniform("VM", wt);
        shader->setUniform("NM", NM);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer());
        glDrawArrays(GL_TRIANGLES, 0, mesh->geometry().verticesIndeces().size());

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}


void TrianglePicker::mesh(std::shared_ptr<Mesh> v) noexcept {
    Model::mesh(v);

    auto size = mesh()->geometry().vertices().size();
    auto color = this->color();
    auto mesh = this->mesh();
    std::vector<GLfloat> colors;
    colors.reserve(size);
    m_colors_amount = size;

    for (decltype(size) i = 0; i < size; i += 3) {
        colors.push_back(color.x);
        colors.push_back(color.y);
        colors.push_back(color.z);
    }

    glGenBuffers(1, &m_colors_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_colors_buffer);
    glBufferData(GL_ARRAY_BUFFER,
            colors.size() * sizeof(decltype(colors)::value_type),
            colors.data(),
            //GL_STATIC_DRAW);
            GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer());
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->normalBuffer());
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, m_colors_buffer);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void TrianglePicker::changeTriangleColor(int id, glm::vec3 _color) {
    if (id < 0 || id >= m_colors_amount/3) return;

    std::vector<GLfloat> tmp;
    tmp.reserve(9);
    for (int i = 0; i < 9; i+=3) {
        tmp[i] = _color.x;
        tmp[i+1] = _color.y;
        tmp[i+2] = _color.z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_colors_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, id * 9 * sizeof(GLfloat), 9 * sizeof(GLfloat), tmp.data());
}


void TrianglePicker::paint(SegmentCollider ray, glm::vec3 _color) {
    auto mesh = this->mesh();
    auto vertices = mesh->geometry().vertices();
    auto size = mesh->geometry().vertices().size();
    auto matrix = worldTransform();
    for (decltype(size) i = 0; i < size; i += 9) {
        glm::vec3 point0{vertices[i  ], vertices[i+1], vertices[i+2]};
        glm::vec3 point1{vertices[i+3], vertices[i+4], vertices[i+5]};
        glm::vec3 point2{vertices[i+6], vertices[i+7], vertices[i+8]};
        point0 = glm::vec3(matrix * glm::vec4(point0, 1));
        point1 = glm::vec3(matrix * glm::vec4(point1, 1));
        point2 = glm::vec3(matrix * glm::vec4(point2, 1));

        int triangle_index = -1;
        if (testIntersection(ray, point0, point1, point2)) {
            triangle_index = i / 9;
        }
        if (triangle_index > -1) {
            changeTriangleColor(triangle_index, _color);
        }
    }
}
