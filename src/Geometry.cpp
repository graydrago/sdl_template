#include <sstream>

#include "../headers/Geometry.h"
#include "../headers/utils.h"

void Geometry::load(std::string fileName) {
    //TODO Add error handling
    using std::getline;
    using std::string;
    using std::istringstream;

    this->_fileName = fileName;

    auto file_strings = loadTextFile(fileName);
    istringstream file(file_strings);
    string line;

    while (getline(file, line)) {
        string type;
        istringstream line_stream(line);

        getline(line_stream, type, ' ');
        if ("v" == type) {
            string num;
            while (getline(line_stream, num, ' ')) {
                _vertices.push_back(::atof(num.c_str()));
            }
        } else if ("vn" == type) {
            string num;
            while (getline(line_stream, num, ' ')) {
                _normals.push_back(::atof(num.c_str()));
            }
        } else if ("f" == type) {
            string pack;
            while (getline(line_stream, pack, ' ')) {
                int pos = pack.find('/');
                string vertex_index_str = pack.substr(0, pos);
                if (pack[pos] != pack[pos+1]) {
                  // TODO Textures
                } else {
                  pos += 2;
                }
                string normal_index_str = pack.substr(pos, pack.length());

                _verticesIndeces.push_back(::atoi(vertex_index_str.c_str()) - 1);
                _normalsIndeces.push_back(::atof(normal_index_str.c_str()) - 1);
            }
        }
    }
}
