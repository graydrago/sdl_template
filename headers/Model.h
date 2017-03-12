#pragma once

#include <iostream>
#include <vector>
#include "ShaderProgram.h"
#include "Geometry.h"

class ModelExeption : public std::runtime_error {
    public:
        ModelExeption(const std::string &msg) :
            std::runtime_error("[ModelExeption] " + msg) {}
};

class Model {
    private:
        std::string fileName;
        Geometry geometry;
        glm::vec3 color;
        ShaderProgram shaderProgram;

        GLuint vertexBufferHandle;
        GLuint normalBufferHandle;
        GLuint vaoHandle;

    public:
        Model();
        virtual ~Model();
        void load(std::string geometryFileName);
        void render(const glm::mat4 &VM, const glm::mat4 &PVM, const glm::mat3 &NM);
};
