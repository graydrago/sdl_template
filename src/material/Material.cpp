#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "../../headers/material/Material.h"
#include "../../headers/utils.h"

Material::Material(glm::vec3 _color) : color(_color) {}


void Material::init() {
    shaderProgram.compile("./assets/basic.frag", GL_FRAGMENT_SHADER);
    shaderProgram.compile("./assets/basic.vert", GL_VERTEX_SHADER);
    shaderProgram.link();
}


void Material::render() {
    shaderProgram.use();
    shaderProgram.setUniform("Color", color);
    shaderProgram.setUniform("PVM", PVM);
}
