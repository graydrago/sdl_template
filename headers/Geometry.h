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
        std::vector<int> m_indices;
        std::string m_fileName;

    public:
        Geometry() {};
        virtual ~Geometry() {};
        bool load(std::string fileName);

        void vertices(std::vector<float> v) { m_vertices = v; }

        std::vector<float>& vertices() { return m_vertices; }
        std::vector<float>& normals() { return m_normals; }
        std::vector<int>& indices() { return m_indices; }
        std::string& fileName() { return m_fileName; }
};
