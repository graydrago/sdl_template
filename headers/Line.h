#pragma once

#include "Model.h"

class Line: public Model {
    public:
        Line() {};
        virtual ~Line() {};

        void render(const glm::mat4 &P, const glm::mat4 &V) const noexcept;
        void changePoints(const glm::vec3 startPoint, const glm::vec3 endPoint);
};
