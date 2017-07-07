#include "../headers/Mesh.h"
#include "../headers/Line.h"

void Line::render(const glm::mat4 &P, const glm::mat4 &V) const noexcept {
    auto mesh = this->mesh();
    auto shader = this->shader();
    if (mesh && shader) {
        auto wt = worldTransform();
        auto PVM = P * V * wt;

        shader->use();
        shader->setUniform("Color", color());
        shader->setUniform("PVM", PVM);

        glBindVertexArray(mesh->VAO());
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer());
        glDrawArrays(GL_LINES, 0, mesh->geometry().vertices().size()/3);
    }
};

void Line::changePoints(const glm::vec3 startPoint, const glm::vec3 endPoint) {
    std::vector<GLfloat> tmp;
    tmp.reserve(6);
    tmp.push_back(startPoint.x);
    tmp.push_back(startPoint.y);
    tmp.push_back(startPoint.z);
    tmp.push_back(endPoint.x);
    tmp.push_back(endPoint.y);
    tmp.push_back(endPoint.z);

    glBindBuffer(GL_ARRAY_BUFFER, mesh()->vertexBuffer());
    glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(GLfloat), tmp.data());
}
