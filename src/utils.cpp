#include <fstream>
#include <SDL2/SDL.h>
#include "../headers/utils.h"

std::string loadTextFile (std::string name) {
    std::ifstream t(name);
    t.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::string str((std::istreambuf_iterator<char>(t)),
            std::istreambuf_iterator<char>());
    std::cout << str.length() << std::endl;
    return str;
}

void gl_print_extentions() {
    PFNGLGETSTRINGIPROC glGetStringi = nullptr;
    glGetStringi = (PFNGLGETSTRINGIPROC) SDL_GL_GetProcAddress("glGetStringi");

    GLint nExtentions = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &nExtentions);
    std::cout << nExtentions << std::endl;

    for (int i = 0; i < nExtentions; i++) {
        const GLubyte *value = glGetStringi(GL_EXTENSIONS, i);
        if (value != nullptr) {
            std::cout << value << std::endl;
        }
    }
}

void gl_info() {
    gl_print_str("GL Renderer", GL_RENDERER);
    gl_print_str("GL Vendor", GL_VENDOR);
    gl_print_str("GL Version (string)", GL_VERSION);
    gl_print_str("GLSL Version", GL_SHADING_LANGUAGE_VERSION);
}

void gl_print_str (std::string name, GLenum _enum) {
    const GLubyte *value = glGetString(_enum);
    if (value == nullptr) {
        std::cerr << "Can't get '" << name << "'" << std::endl;
    } else {
        std::cout << name << ": " << value << std::endl;
    }
};
