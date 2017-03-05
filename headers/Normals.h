#pragma once

#include <iostream>
#include <vector>
#include <GL/gl.h>
#include "Model.h"
#include "ShaderProgram.h"
#include "../include/glm/glm/glm.hpp"

class NormalsExeption : public std::runtime_error {
    public:
        NormalsExeption(const std::string &msg) :
            std::runtime_error(msg) {}
};

class Normals {
    //private:
    public:
        GLuint indexBufferHandle;
        GLuint vertexBufferHundle;
        GLuint normalBufferHandle;
        ShaderProgram program;
        GLuint vaoHandle;
        std::vector<float> vertices;
        std::vector<int> indeces;
        std::vector<float> normals;
        std::string fileName;

        Normals(Model model);
        virtual ~Normals() {};
        void render(glm::mat4 PVM);
};
