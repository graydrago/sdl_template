#include <GL/glew.h>
#include "../headers/Normals.h"


Normals::Normals(Model model) {
    using std::cout;
    using std::endl;

    if (model.indeces.size() == 0 && model.vertices.size() == model.normals.size()) {
        for (int i = 0; i < (int) model.vertices.size(); i += 3) {
            glm::vec3 start_point(model.vertices[i], model.vertices[i+1], model.vertices[i+2]);
            glm::vec3 direction(model.normals[i], model.normals[i+1], model.normals[i+2]);
            glm::vec3 end_point = start_point + (direction * glm::vec3(0.1, 0.1, 0.1));

            vertices.push_back(start_point.x);
            vertices.push_back(start_point.y);
            vertices.push_back(start_point.z);
            vertices.push_back(end_point.x);
            vertices.push_back(end_point.y);
            vertices.push_back(end_point.z);
        }
    } else {
        for (int i = 0; i < (int) model.indeces.size(); i++) {
            float x = model.vertices[model.indeces[i]*3];
            float y = model.vertices[model.indeces[i]*3+1];
            float z = model.vertices[model.indeces[i]*3+2];

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            glm::vec3 start_point(x, y, z);
            glm::vec3 direction(model.normals[i*3], model.normals[i*3+1], model.normals[i*3+2]);
            glm::vec3 end_point = start_point + (direction * glm::vec3(0.1, 0.1, 0.1));

            vertices.push_back(end_point.x);
            vertices.push_back(end_point.y);
            vertices.push_back(end_point.z);
        }
    }

    cout << "Normals Vertices: " << vertices.size() << endl;


    glGenBuffers(1, &vertexBufferHundle);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHundle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &(vertices)[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHundle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    program.compile("./assets/basic.frag", GL_FRAGMENT_SHADER);
    program.compile("./assets/basic.vert", GL_VERTEX_SHADER);
    program.link();
}

void Normals::render(glm::mat4 PVM) {
    program.use();
    program.setUniform("Color", glm::vec3(0.f, 0.f, 1.f));
    program.setUniform("PVM", PVM);

    glBindVertexArray(vaoHandle);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBufferHundle);
    glDrawArrays(GL_LINES, 0, vertices.size());
}
