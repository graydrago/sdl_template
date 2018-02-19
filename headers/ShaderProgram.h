#pragma once

#include <iostream>
#include <EGL/egl.h>
#include <glm/matrix.hpp>
#include "../headers/utils.h"

class ShaderProgramExeption : public std::runtime_error {
    public:
        ShaderProgramExeption(const std::string &msg) :
            std::runtime_error(msg) {}
};

class ShaderProgram {
    public:
        GLuint handle;
        ShaderProgram();
        virtual ~ShaderProgram();
        void compile(const std::string &fileName, const GLuint shaderType, const bool isFile = true);
        void link();
        void use();
        void bindAttribLocation(const GLuint location, const std::string name);
        void setUniform(const std::string &name, const int value);
        void setUniform(const std::string &name, const float value);
        void setUniform(const std::string &name, const glm::mat3 &value);
        void setUniform(const std::string &name, const glm::mat4 &value);
        void setUniform(const std::string &name, const glm::vec2 &value);
        void setUniform(const std::string &name, const glm::vec3 &value);
        void setUniform(const std::string &name, const glm::vec4 &value);
};
