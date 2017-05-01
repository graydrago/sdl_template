#pragma once

#include <GL/glew.h>
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
        Mesh() {};
        virtual ~Mesh() {};
        void load(std::string geometryFileName);

        GLuint vertexBuffer() { return m_vertexBufferHandle; };
        GLuint normalBuffer() { return m_normalBufferHandle; };
        GLuint vaoHandle() { return m_vaoHandle; };
        Geometry& geometry() { return m_geometry; };
};
