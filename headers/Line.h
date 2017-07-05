#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <functional>

#include "Model.h"

class Line: public Model {
    public:
        Line() {};
        virtual ~Line() {};

        void render(const glm::mat4 &P, const glm::mat4 &V) const noexcept {
            auto mesh = getMesh();
            auto shader = getShader();
            if (mesh && shader) {
                auto wt = getWorldTransform();
                auto PVM = P * V * wt;

                shader->use();
                shader->setUniform("Color", getColor());
                shader->setUniform("PVM", PVM);

                glBindVertexArray(mesh->vaoHandle());
                glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer());
                glDrawArrays(GL_LINES, 0, mesh->geometry().vertices().size()/3);
            }
        };
};
