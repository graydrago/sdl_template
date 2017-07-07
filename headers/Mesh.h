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
        GLuint m_vertexBufferHandle;
        GLuint m_normalBufferHandle;
        GLuint m_vaoHandle;
        Geometry m_geometry;

    public:
        Mesh() :
            m_vertexBufferHandle(0),
            m_normalBufferHandle(0),
            m_vaoHandle(0) {};
        virtual ~Mesh() {
            if (m_normalBufferHandle > 0) glDeleteBuffers(1, &m_vertexBufferHandle);
            if (m_normalBufferHandle > 0) glDeleteBuffers(1, &m_normalBufferHandle);
            if (m_vaoHandle > 0) glDeleteVertexArrays(1, &m_vaoHandle);
        };
        void load(std::string geometryFileName);

        GLuint vertexBuffer() noexcept { return m_vertexBufferHandle; }
        GLuint normalBuffer() noexcept { return m_normalBufferHandle; }
        Geometry& geometry() noexcept { return m_geometry; }
        GLuint VAO() noexcept { return m_vaoHandle; } 

        void makeVertexBuffer(std::vector<float> v, GLuint draw_type = GL_STATIC_DRAW);
        void makeNormalBuffer(std::vector<float> n, GLuint draw_type = GL_STATIC_DRAW);
        void makeVAO(GLuint size = 3);

};
