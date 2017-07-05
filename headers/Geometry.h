#pragma once

#include <iostream>
#include <vector>

class GeometryExeption : public std::runtime_error {
    public:
        GeometryExeption(const std::string &msg) :
            std::runtime_error("[GeometryExeption] " + msg) {}
};

class Geometry {
    private:
        std::vector<float> m_vertices;
        std::vector<float> m_normals;
        std::vector<int> m_verticesIndeces;
        std::vector<int> m_normalsIndeces;
        std::string m_fileName;

    public:
        Geometry() {};
        virtual ~Geometry() {};
        void load(std::string fileName);

        void vertices(std::vector<float> v) { m_vertices = v; }

        std::vector<float>& vertices() { return m_vertices; }
        std::vector<float>& normals() { return m_normals; }
        std::vector<int>& verticesIndeces() { return m_verticesIndeces; }
        std::vector<int>& normalsIndeces() { return m_normalsIndeces; }
        std::string& fileName() { return m_fileName; }
};
