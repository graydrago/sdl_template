#pragma once

#include <iostream>
#include <vector>

class ModelExeption : public std::runtime_error {
    public:
        ModelExeption(const std::string &msg) :
            std::runtime_error(msg) {}
};

class Model {
    //private:
    public:
        std::vector<float> vertices;
        std::vector<int> indeces;
        std::vector<float> faces;
    //public:
        Model();
        virtual ~Model() {};
        void load(std::string fileName);
};