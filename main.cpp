/* NOT FINISHED */
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glext.h>

const int SCREEN_W {800};
const int SCREEN_H {600};

void gl_print_str (std::string name, GLenum _enum);
void gl_info();
void gl_print_extentions();

int main() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Unable to init SDL: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

    SDL_Window* win = SDL_CreateWindow("Shooter",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            SCREEN_W, SCREEN_H,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (win == nullptr) {
        std::cout << "Unable to create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GLContext ctx = SDL_GL_CreateContext(win);
    if (ctx == nullptr) {
        std::cout << "Unable to create GL Context: " << SDL_GetError() << std::endl;
        return 1;
    }
    gl_info();
    gl_print_extentions();

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
        std::cout << "Can't get '" << name << "'" << std::endl;
    } else {
        std::cout << name << ": " << value << std::endl;
    }
};
