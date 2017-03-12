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
        std::vector<float> _vertices;
        std::vector<float> _normals;
        std::vector<int> _verticesIndeces;
        std::vector<int> _normalsIndeces;
        std::string _fileName;

    public:
        Geometry() {};
        virtual ~Geometry() {};
        void load(std::string fileName);

        std::vector<float>& vertices() { return _vertices; }
        std::vector<float>& normals() { return _normals; }
        std::vector<int>& verticesIndeces() { return _verticesIndeces; }
        std::vector<int>& normalsIndeces() { return _normalsIndeces; }
        std::string& fileName() { return _fileName; }
};
