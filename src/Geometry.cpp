#include <fstream>
#include <algorithm>
#include <SDL2/SDL_log.h>
#include <json.hpp>

#include "../headers/Geometry.h"
#include "../headers/utils.h"

bool Geometry::load(std::string fileName) {
    using json = nlohmann::json;
    this->m_fileName = fileName;
    std::ifstream f(fileName);
    if (!f.is_open()) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Can't load a file: %s", fileName.c_str());
        return false;
    }

    auto j = json::parse(f);
    m_vertices.reserve(j["vertices"].size());
    m_normals.reserve(j["normals"].size());
    m_indices.reserve(j["indices"].size());
    for (auto i : j["vertices"]) m_vertices.push_back(i);
    for (auto i : j["normals"]) m_normals.push_back(i);
    for (auto i : j["indices"]) m_indices.push_back(i);

    return true;
}
