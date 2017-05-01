#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include "ShaderProgram.h"
#include "Mesh.h"

class ModelExeption : public std::runtime_error {
    public:
        ModelExeption(const std::string &msg) :
            std::runtime_error("[ModelExeption] " + msg) {}
};

class Model {
    private:
        std::string fileName;
        glm::vec3 m_color;
        std::shared_ptr<Mesh> m_mesh;
        std::shared_ptr<ShaderProgram> m_shaderProgram;
        std::function<void(Model&, float)> update_cb;
        std::vector<std::unique_ptr<Model>> children;
        Geometry geometry;

        GLuint vertexBufferHandle;
        GLuint normalBufferHandle;
        GLuint vaoHandle;
        bool has_loaded_movel;

    public:
        glm::mat4 matrix;


        Model();
        virtual ~Model();
        void load(std::string geometryFileName);
        void setGeometry(std::shared_ptr<Geometry> geometry);
        void setMesh(std::shared_ptr<Mesh> mesh);
        void attachShader(std::shared_ptr<ShaderProgram> shader);
        void update(float elapsed_time);
        void render(const glm::mat4 &V, const glm::mat4 &P);
        void setUpdateCb(std::function<void(Model&, float)> cb);
        void addChild(std::unique_ptr<Model> model);
        void color(const glm::vec3& _color) noexcept;
};
