#include <fstream>
#include <SDL2/SDL.h>
#include "../headers/utils.h"

std::string loadTextFile(std::string name) {
    std::ifstream t(name);
    //t.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::string str((std::istreambuf_iterator<char>(t)),
            std::istreambuf_iterator<char>());
    std::cout << name << " size: " << str.length() << std::endl;
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


void gl_print_str(std::string name, GLenum _enum) {
    const GLubyte *value = glGetString(_enum);
    if (value == nullptr) {
        std::cerr << "Can't get '" << name << "'" << std::endl;
    } else {
        std::cout << name << ": " << value << std::endl;
    }
};


const char* gl_get_error_string(GLenum err) {
    switch (err) {
        case GL_NO_ERROR:          return "No error";
        case GL_INVALID_ENUM:      return "Invalid enum";
        case GL_INVALID_VALUE:     return "Invalid value";
        case GL_INVALID_OPERATION: return "Invalid operation";
        case GL_STACK_OVERFLOW:    return "Stack overflow";
        case GL_STACK_UNDERFLOW:   return "Stack underflow";
        case GL_OUT_OF_MEMORY:     return "Out of memory";
        default:                   return "Unknown error";
    }
}


void gl_check_error() {
    const GLenum err = glGetError();
    if (GL_NO_ERROR != err) {
        std::cout << "GL Error:" << err << ":" << gl_get_error_string(err) << std::endl;
    }
}
