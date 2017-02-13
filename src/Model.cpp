#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "../headers/Model.h"
#include "../headers/utils.h"

Model::Model() {
}

void Model::load(std::string fileName) {
    //TODO Add error handling
    using std::getline;
    using std::string;
    using std::istringstream;

    std::ifstream file(fileName);
    string line;

    while (getline(file, line)) {
        string type;
        istringstream line_stream(line);

        getline(line_stream, type, ' ');
        if ("v" == type) {
            string num;
            while (getline(line_stream, num, ' ')) {
                vertices.push_back(::atof(num.c_str()));
            }
        } else if ("f" == type) {
            string pack;
            while (getline(line_stream, pack, ' ')) {
                string index;
                getline(istringstream(pack), index, '/');
                indeces.push_back(::atoi(index.c_str()) - 1);
            }
        }
    }
}
