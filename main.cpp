/* NOT FINISHED */
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/gl.h>

const int SCREEN_W {800};
const int SCREEN_H {600};

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Unable to init SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

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

    // This makes our buffer swap syncronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float ratio = (float)SCREEN_W / (float)SCREEN_H;
    glOrtho(-ratio, ratio, 1.0f, -1.0f, 0.f, 10.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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
