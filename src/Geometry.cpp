#include <sstream>
#include <SDL2/SDL_log.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../headers/Geometry.h"
#include "../headers/utils.h"

bool Geometry::load(std::string fileName) {
    this->m_fileName = fileName;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(fileName,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    if (!scene) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", importer.GetErrorString());
        return false;
    }

    auto mesh = scene->mMeshes[0];

    m_vertices.reserve(mesh->mNumVertices * 3);
    m_normals.reserve(mesh->mNumVertices * 3);
    m_verticesIndeces.reserve(mesh->mNumFaces * 3);
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        auto v = mesh->mVertices[i];
        m_vertices.push_back(v.x);
        m_vertices.push_back(v.y);
        m_vertices.push_back(v.z);
        auto n = mesh->mNormals[i];
        m_normals.push_back(n.x);
        m_normals.push_back(n.y);
        m_normals.push_back(n.z);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        auto f = mesh->mFaces[i];
        for (unsigned int j = 0; j < f.mNumIndices; ++j) {
            m_verticesIndeces.push_back(f.mIndices[j]);
        }
    }

    return true;
}
