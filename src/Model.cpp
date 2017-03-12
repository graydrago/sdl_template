#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "../headers/Model.h"
#include "../headers/utils.h"


Model::Model() {
    color = glm::vec3(0.f, 0.f, 1.f);
}


Model::~Model() {}


void Model::load(std::string geometryFileName) {
    using std::cout;
    using std::endl;

    std::vector<float> vertices;
    std::vector<float> normals;

    geometry.load(geometryFileName);

    for (auto item : geometry.verticesIndeces()) {
        vertices.push_back(geometry.vertices()[item * 3 ]);
        vertices.push_back(geometry.vertices()[item * 3 + 1]);
        vertices.push_back(geometry.vertices()[item * 3 + 2]);
    }
    for (auto item : geometry.normalsIndeces()) {
        normals.push_back(geometry.normals()[item * 3 ]);
        normals.push_back(geometry.normals()[item * 3 + 1]);
        normals.push_back(geometry.normals()[item * 3 + 2]);
    }

    glGenBuffers(1, &vertexBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,
            vertices.size() * sizeof(float),
            &(vertices)[0],
            GL_STATIC_DRAW);

    glGenBuffers(1, &normalBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,
            normals.size() * sizeof(float),
            &(normals)[0],
            GL_STATIC_DRAW);

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    cout << vertices.size() << endl;
    cout << normals.size() << endl;

    shaderProgram.compile("./assets/onePointLight.frag", GL_FRAGMENT_SHADER);
    shaderProgram.compile("./assets/onePointLight.vert", GL_VERTEX_SHADER);
    shaderProgram.link();

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    gl_check_error();
}

void Model::render(const glm::mat4 &VM, const glm::mat4 &PVM, const glm::mat3 &NM) {
    shaderProgram.use();
    shaderProgram.setUniform("Color", color);
    shaderProgram.setUniform("PVM", PVM);
    shaderProgram.setUniform("VM", VM);
    shaderProgram.setUniform("NM", NM);

    glBindVertexArray(vaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle);

    glDrawArrays(GL_TRIANGLES, 0, geometry.verticesIndeces().size());
}
