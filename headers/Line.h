#pragma once

#include <memory>
#include <functional>

#include "Model.h"
#include "Mesh.h"

class Line: public Model {
    public:
        Line() {};
        virtual ~Line() {};

        void render(const glm::mat4 &P, const glm::mat4 &V) const noexcept {
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
};
