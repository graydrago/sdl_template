#include "../headers/Mesh.h"
#include "../headers/utils.h"

void Mesh::load(std::string geometryFileName) {
    using std::cout;
    using std::endl;

    std::vector<float> vertices;
    std::vector<float> normals;

    m_geometry.load(geometryFileName);

    for (auto item : m_geometry.indices()) {
        vertices.push_back(m_geometry.vertices()[item * 3 ]);
        vertices.push_back(m_geometry.vertices()[item * 3 + 1]);
        vertices.push_back(m_geometry.vertices()[item * 3 + 2]);
    }
    for (auto item : m_geometry.indices()) {
        normals.push_back(m_geometry.normals()[item * 3 ]);
        normals.push_back(m_geometry.normals()[item * 3 + 1]);
        normals.push_back(m_geometry.normals()[item * 3 + 2]);
    }

    std::cout << "geometry file name: " << geometryFileName << std::endl;
    std::cout << "vertices: " << vertices.size() << std::endl;
    std::cout << "normals: " << normals.size() << std::endl;

    makeVertexBuffer(vertices);
    makeNormalBuffer(normals);
    gl_check_error();
}


void Mesh::makeVertexBuffer(std::vector<float> vertices, GLuint draw_type) {
    if (m_normalBuffer > 0) {
      glDeleteBuffers(1, &m_vertexBuffer);
    }
    m_geometry.vertices(vertices);

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(decltype(vertices)::value_type),
                 vertices.data(),
                 draw_type);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Mesh::makeNormalBuffer(std::vector<float> normals, GLuint draw_type) {
    if (m_normalBuffer > 0) glDeleteBuffers(1, &m_normalBuffer);

    glGenBuffers(1, &m_normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
    glBufferData(GL_ARRAY_BUFFER,
            normals.size() * sizeof(decltype(normals)::value_type),
            normals.data(),
            draw_type);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

