#pragma once

#include <iostream>
#include <GL/gl.h>
#include "../headers/utils.h"

class ShaderProgramExeption : public std::runtime_error {
    public:
        ShaderProgramExeption(const std::string &msg) :
            std::runtime_error(msg) {};
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
};
