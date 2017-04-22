#pragma once

#include <iostream>
#include <vector>
#include <memory>
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
        glm::vec3 m_color;
        std::shared_ptr<ShaderProgram> shaderProgram;
        std::vector<std::unique_ptr<Model>> children;

        GLuint vertexBufferHandle;
        GLuint normalBufferHandle;
        GLuint vaoHandle;

    public:
        glm::mat4 local_matrix;
        glm::mat4 global_matrix;

        Model();
        virtual ~Model();
        void load(std::string geometryFileName);
        void attachShader(std::shared_ptr<ShaderProgram> shader);
        void render(const glm::mat4 &V, const glm::mat4 &P);
        void addChild(std::unique_ptr<Model> model);
        void color(const glm::vec3& _color) noexcept;
};
