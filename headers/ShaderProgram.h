#pragma once

#include <iostream>
#include <GL/gl.h>
#include <glm/matrix.hpp>
#include "../headers/utils.h"

class ShaderProgramExeption : public std::runtime_error {
    public:
        ShaderProgramExeption(const std::string &msg) :
            std::runtime_error(msg) {}
};

class ShaderProgram {
    private:
        GLuint handle;

    public:
        ShaderProgram();
        virtual ~ShaderProgram();
        void compile(std::string fileName, GLuint shaderType);
        void link();
        void use();
        void bindAttribLocation(const GLuint location, const std::string name);
        void setUniform(const std::string &name, const int value);
        void setUniform(const std::string &name, const float value);
        void setUniform(const std::string &name, const glm::mat3 &value);
        void setUniform(const std::string &name, const glm::mat4 &value);
};
