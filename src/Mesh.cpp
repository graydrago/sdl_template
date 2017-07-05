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

    makeVertexBuffer(vertices);
    makeNormalBuffer(normals);
    makeVAO();
    gl_check_error();
}


void Mesh::makeVertexBuffer(std::vector<float> vertices, GLuint draw_type) {
    if (m_normalBufferHandle > 0) {
      glDeleteBuffers(1, &m_vertexBufferHandle);
    }
    m_geometry.vertices(vertices);

    glGenBuffers(1, &m_vertexBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(decltype(vertices)::value_type),
                 vertices.data(),
                 draw_type);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Mesh::makeNormalBuffer(std::vector<float> normals, GLuint draw_type) {
    if (m_normalBufferHandle > 0) glDeleteBuffers(1, &m_normalBufferHandle);

    glGenBuffers(1, &m_normalBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferHandle);
    glBufferData(GL_ARRAY_BUFFER,
            normals.size() * sizeof(decltype(normals)::value_type),
            normals.data(),
            draw_type);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Mesh::makeVAO(GLuint size) {
    if (m_vaoHandle > 0) glDeleteVertexArrays(1, &m_vaoHandle);
    bool has_normals = m_geometry.normals().size() > 0;

    glGenVertexArrays(1, &m_vaoHandle);
    glBindVertexArray(m_vaoHandle);
    glEnableVertexAttribArray(0);

    if (has_normals) {
      glEnableVertexAttribArray(1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
    glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, NULL);

    if (has_normals) {
      glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferHandle);
      glVertexAttribPointer(1, size, GL_FLOAT, GL_FALSE, 0, NULL);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

