/* NOT FINISHED */
#define GLEW_STATIC

#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <fstream>
#include <streambuf>
#include <vector>

const int SCREEN_W {800};
const int SCREEN_H {600};

void gl_print_str (std::string name, GLenum _enum);
void gl_info();
void gl_print_extentions();
std::string load_text_file (std::string name);
GLuint load_shader(GLenum shader_type, std::string file_name);
GLuint create_shader_program(std::vector<GLuint> shader_list);

int main() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Unable to init SDL: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

    SDL_Window* win = SDL_CreateWindow("Template",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            SCREEN_W, SCREEN_H,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (win == nullptr) {
        std::cerr << "Unable to create window: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_GLContext ctx = SDL_GL_CreateContext(win);
    if (ctx == nullptr) {
        std::cerr << "Unable to create GL Context: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "GLEW's been inited." << std::endl;

    gl_info();
    gl_print_extentions();
    GLuint vertex_shader = load_shader(GL_VERTEX_SHADER, "./assets/default.vert");
    GLuint fragment_shader = load_shader(GL_FRAGMENT_SHADER, "./assets/default.frag");
    std::vector<GLuint> shaders {vertex_shader, fragment_shader};
    GLuint shader_program = create_shader_program(shaders);
    glUseProgram(shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // This makes our buffer swap syncronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //float ratio = (float)SCREEN_W / (float)SCREEN_H;

    bool play = true;

    while (play) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                play = false;
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                    case SDLK_q:
                        play = false;
                        break;
                }
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 0.22f, 0.0f); // Coquelicot #FF3800 RGB:100% 22% 0%
        glRectf(-0.5, 0.5, 0.5, -0.5);

        SDL_GL_SwapWindow(win);
    }

    glUseProgram(0);
    glDeleteProgram(shader_program);

    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);
    SDL_Quit();
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

std::string load_text_file (std::string name) {
    std::ifstream t(name);
    t.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::string str((std::istreambuf_iterator<char>(t)),
            std::istreambuf_iterator<char>());
    std::cout << str.length() << std::endl;
    return str;
}

GLuint load_shader(GLenum shader_type, std::string file_name) {
    GLuint shader = glCreateShader(shader_type);
    if (shader == 0) {
        std::cerr << "Can't create a vertex shader\n";
        exit(EXIT_FAILURE);
    }

    std::string shaderText = load_text_file(file_name);
    const GLchar *shaderCode = shaderText.c_str();
    const GLchar *codeArray[] = {shaderCode};
    glShaderSource(shader, 1, codeArray, NULL);
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (GL_FALSE == result) {
        std::cerr << "Can't compile a shader: " << file_name << std::endl;

        GLint log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

        GLchar *log_text = new GLchar[log_length];
        glGetShaderInfoLog(shader, log_length, nullptr, log_text);

        std::cerr << "----- Start Shader Log -----" << std::endl;
        std::cerr << log_text << std::endl;
        std::cerr << "----- End Shader Log -----" << std::endl;

        delete [] log_text;
        exit(EXIT_FAILURE);
    }

    return shader;
}

GLuint create_shader_program(std::vector<GLuint> shader_list) {
    GLuint program = glCreateProgram();
    if (program == 0) {
        std::cerr << "Can't create a program" << std::endl;
        exit(EXIT_FAILURE);
    }

    for (auto shader : shader_list) {
        glAttachShader(program, shader);
    }

    glLinkProgram(program);

    GLint result;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (GL_FALSE == result) {
        std::cerr << "Can't link a shader program." << std::endl;

        GLint log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

        if (log_length > 0) {
            GLchar *log_text = new GLchar[log_length];
            glGetProgramInfoLog(program, log_length, nullptr, log_text);
            std::cerr << "----- Start Shader Program Log -----" << std::endl;
            std::cerr << log_text << std::endl;
            std::cerr << "----- End Shader Program Log -----" << std::endl;
            delete [] log_text;
        }

        exit(EXIT_FAILURE);
    }

    return program;
}

