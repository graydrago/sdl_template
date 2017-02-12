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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./headers/utils.h"
#include "./headers/ShaderProgram.h"

const int SCREEN_W {800};
const int SCREEN_H {600};


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

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "GLEW's been inited." << std::endl;

    gl_info();
    gl_print_extentions();

    // This makes our buffer swap syncronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //float ratio = (float)SCREEN_W / (float)SCREEN_H;

    bool play = true;

    float positionData[] = {
        -0.8f, -0.8f, 0.0f,
         0.8f, -0.8f, 0.0f,
         0.0f,  0.8f, 0.0f,
    };

    float colorData[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    GLuint vaoHandle;
    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);
    GLuint positonBufferHundle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];

    glBindBuffer(GL_ARRAY_BUFFER, positonBufferHundle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

    ShaderProgram program;
    program.compile("./assets/basic.frag", GL_FRAGMENT_SHADER);
    program.compile("./assets/basic.vert", GL_VERTEX_SHADER);
    program.link();
    program.use();

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, positonBufferHundle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

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
        Uint32 elapsed = SDL_GetTicks();
        program.setUniform("Scale", glm::sin((float) elapsed * 0.001f)*1.f);
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians((float) (elapsed/10 % 360)), glm::vec3(0.f, 0.f, 1.f));
        program.setUniform("Rotation", rotate);
        glBindVertexArray(vaoHandle);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(win);
    }

    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

