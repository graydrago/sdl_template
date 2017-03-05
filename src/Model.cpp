#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "../headers/Model.h"
#include "../headers/utils.h"

Model::Model() {}

void Model::load(std::string fileName) {
    //TODO Add error handling
    using std::getline;
    using std::string;
    using std::istringstream;
    using std::cout;
    using std::endl;

    this->fileName = fileName;

    auto file_strings = loadTextFile(fileName);
    std::istringstream file(file_strings);
    string line;
    std::vector<float> tmp_vertices;
    std::vector<float> tmp_normals;

    while (getline(file, line)) {
        string type;
        istringstream line_stream(line);

        getline(line_stream, type, ' ');
        if ("v" == type) {
            string num;
            while (getline(line_stream, num, ' ')) {
                tmp_vertices.push_back(::atof(num.c_str()));
            }
        } else if ("vn" == type) {
            string num;
            while (getline(line_stream, num, ' ')) {
                tmp_normals.push_back(::atof(num.c_str()));
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

                int vertex_index = ::atoi(vertex_index_str.c_str()) - 1;
                int normal_index = ::atof(normal_index_str.c_str()) - 1;

                vertices.push_back(tmp_vertices[vertex_index * 3]);
                vertices.push_back(tmp_vertices[vertex_index * 3 + 1]);
                vertices.push_back(tmp_vertices[vertex_index * 3 + 2]);

                normals.push_back(tmp_normals[normal_index * 3]);
                normals.push_back(tmp_normals[normal_index * 3 + 1]);
                normals.push_back(tmp_normals[normal_index * 3 + 2]);
            }
        }
    }

    for (auto i : indeces) {
      cout << "id " << i << "\n";
      cout << "v(" << vertices[i*3] << ", "
                   << vertices[i*3+1]  << ", "
                   << vertices[i*3+2]  << ")\n";
      cout << "n(" << normals[i*3] << ", "
                   << normals[i*3+1]  << ", "
                   << normals[i*3+2]  << ")\n";
    }
}

void Model::printInfo() {
  using std::cout;
  using std::endl;

  cout << "Model: " << fileName << endl;
  cout << "vertices: " << vertices.size() << endl;
  cout << "indeces: " << indeces.size() << endl;
  cout << "normals: " << normals.size() << endl;
}
