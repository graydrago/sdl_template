#pragma once

#include <iostream>
#include <vector>
#include <glm/vec3.hpp>

#include "../../headers/ShaderProgram.h"

class MaterialExeption : public std::runtime_error {
    public:
        MaterialExeption(const std::string &msg) :
            std::runtime_error("[MaterialExeption] " + msg) {}
};

class Material {
    //private:
    public:
        std::vector<float> vertices;
        std::vector<int> indeces;
        std::vector<float> normals;
        glm::vec3 color;
        glm::vec3 PVM;
        ShaderProgram shaderProgram;

        Material(glm::vec3 color);
        virtual ~Material() {};
        virtual void init();
        virtual void render();
};
