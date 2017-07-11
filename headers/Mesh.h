#pragma once

#include <GL/glew.h>
#include <vector>
#include "Geometry.h"

class MeshExeption : public std::runtime_error {
    public:
        MeshExeption(const std::string &msg) :
            std::runtime_error("[MeshExeption] " + msg) {}
};

class Mesh {
    private:
        std::string m_fileName;
        GLuint m_vertexBuffer;
        GLuint m_normalBuffer;
        Geometry m_geometry;

    public:
        Mesh() :
            m_vertexBuffer(0),
            m_normalBuffer(0) {}
        virtual ~Mesh() {
            glDeleteBuffers(1, &m_vertexBuffer);
            glDeleteBuffers(1, &m_normalBuffer);
        };
        void load(std::string geometryFileName);

        GLuint vertexBuffer() noexcept { return m_vertexBuffer; }
        GLuint normalBuffer() noexcept { return m_normalBuffer; }
        Geometry& geometry() noexcept { return m_geometry; }

        void makeVertexBuffer(std::vector<float> v, GLuint draw_type = GL_STATIC_DRAW);
        void makeNormalBuffer(std::vector<float> n, GLuint draw_type = GL_STATIC_DRAW);
};
