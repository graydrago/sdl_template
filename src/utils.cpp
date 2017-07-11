#define GLM_ENABLE_EXPERIMENTAL

#include <fstream>
#include <SDL2/SDL.h>
#include <glm/gtx/intersect.hpp>

#include "../headers/utils.h"
#include "../headers/SegmentCollider.h"
#include "../headers/SphereCollider.h"

std::string loadTextFile(std::string name) {
    std::ifstream f;
    f.open(name);
    if (!f.is_open()) {
        throw std::runtime_error("Can't load a file: " + name);
    }

    f.seekg(0, f.end);
    int fileLength = f.tellg();
    f.seekg(0, f.beg);

    char *fileData = new char[fileLength+1];
    f.read(fileData, fileLength);
    fileData[fileLength] = '\0';

    std::string str(fileData);
    delete [] fileData;

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


bool testIntersection(const SegmentCollider& seg, const SphereCollider& sph) noexcept {
    auto result {false};

    glm::vec3 intPoint0;
    glm::vec3 intPoint1;
    glm::vec3 intNormal0;
    glm::vec3 intNormal1;
    result = glm::intersectLineSphere(
        seg.startPoint(), seg.endPoint(),
        sph.center(), sph.radius(),
        intPoint0, intNormal0,
        intPoint1, intNormal1);

    return result;
}


bool testIntersection(const SegmentCollider& seg, const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2) noexcept {
    glm::vec3 intPoint;
    return testIntersection(seg, point0, point1, point2, intPoint);
}


bool testIntersection(const SegmentCollider& seg, const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2, glm::vec3& int_point) noexcept
{
    auto result {false};

    result = glm::intersectLineTriangle(
        seg.startPoint(), glm::normalize(seg.endPoint() - seg.startPoint()),
        point0, point1, point2,
        int_point);

    return result;
}
