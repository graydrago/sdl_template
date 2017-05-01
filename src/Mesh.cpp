#include "../headers/Mesh.h"
#include "../headers/utils.h"

void Mesh::load(std::string geometryFileName) {
    using std::cout;
    using std::endl;

    std::vector<float> vertices;
    std::vector<float> normals;

    m_geometry.load(geometryFileName);

    for (auto item : m_geometry.verticesIndeces()) {
        vertices.push_back(m_geometry.vertices()[item * 3 ]);
        vertices.push_back(m_geometry.vertices()[item * 3 + 1]);
        vertices.push_back(m_geometry.vertices()[item * 3 + 2]);
    }
    for (auto item : m_geometry.normalsIndeces()) {
        normals.push_back(m_geometry.normals()[item * 3 ]);
        normals.push_back(m_geometry.normals()[item * 3 + 1]);
        normals.push_back(m_geometry.normals()[item * 3 + 2]);
    }

    glGenBuffers(1, &m_vertexBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,
            vertices.size() * sizeof(float),
            &(vertices)[0],
            GL_STATIC_DRAW);

    glGenBuffers(1, &m_normalBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,
            normals.size() * sizeof(float),
            &(normals)[0],
            GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_vaoHandle);
    glBindVertexArray(m_vaoHandle);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferHandle);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    cout << vertices.size() << endl;
    cout << normals.size() << endl;

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    gl_check_error();
}
