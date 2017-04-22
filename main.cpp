/* NOT FINISHED */
#define GLEW_STATIC

#include <GL/glew.h>

#ifdef __EMSCRIPTEN__
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <emscripten.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#include <iostream>
#include <SDL2/SDL.h>
#include <fstream>
#include <streambuf>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./headers/Model.h"
#include "./headers/Normals.h"

const int SCREEN_W {800};
const int SCREEN_H {600};

std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> win {
  nullptr, [](SDL_Window* p) { SDL_DestroyWindow(p); SDL_Quit(); }
};
std::unique_ptr<Model> globalModel {nullptr};
bool play = true;

void loop();

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Unable to init SDL: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    #ifdef __EMSCRIPTEN__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    #else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    #endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

    win.reset(SDL_CreateWindow(
        "Template",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL));
    if (win == nullptr) {
        std::cerr << "Unable to create window: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    #ifdef TEST_SDL_LOCK_OPTS
    EM_ASM("SDL.defaults.copyOnLock = false; SDL.defaults.discardOnLock = true; SDL.defaults.opaqueFrontBuffer = false;");
    #endif

    std::unique_ptr<void, void(*)(void* p)> ctx(
      SDL_GL_CreateContext(win.get()),
      [](void* p) { SDL_GL_DeleteContext(p); }
    );
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
    // If there is "Invalid enum", then it's ok.
    // https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library
    gl_check_error();

    glEnable(GL_DEPTH_TEST);

    gl_info();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    std::shared_ptr<ShaderProgram> shaderProgram(new ShaderProgram());
    #ifdef __EMSCRIPTEN__
    shaderProgram->compile("./assets/es2/es_onePointLight.frag", GL_FRAGMENT_SHADER);
    shaderProgram->compile("./assets/es2/es_onePointLight.vert", GL_VERTEX_SHADER);
    #else
    shaderProgram->compile("./assets/onePointLight.frag", GL_FRAGMENT_SHADER);
    shaderProgram->compile("./assets/onePointLight.vert", GL_VERTEX_SHADER);
    #endif
    shaderProgram->link();

    globalModel.reset(new Model);
    globalModel->load("./assets/models/monkey.obj");
    globalModel->attachShader(shaderProgram);


    for (int i = -5; i <= 5; i++) {
      std::unique_ptr<Model> cube{new Model()};
      cube->load("./assets/models/cube.obj");
      cube->attachShader(shaderProgram);
      cube->local_matrix = glm::mat4(1.f);
      cube->local_matrix = glm::scale(cube->local_matrix, glm::vec3(0.2, 0.2, 0.2));
      cube->local_matrix = glm::translate(
          cube->local_matrix,
          //glm::vec3(std::rand()*3.0, std::rand()*3.0, std::rand()*3.0));
          glm::vec3(i, i, i));
      float tc = (i+5.0)/10.0;
      cube->color(glm::vec3(tc, tc, tc));
      globalModel->addChild(std::move(cube));
    }

    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
    #else
    while (play) {
        loop();
    }
    #endif
}


void loop() {
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

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Uint32 elapsed = SDL_GetTicks();

  globalModel->global_matrix = glm::mat4(1.f);

  globalModel->local_matrix = glm::mat4(1.f);
  globalModel->local_matrix = glm::rotate(
      globalModel->local_matrix,
      glm::radians((float) (elapsed/10 % 360)), glm::vec3(0.f, 1.f, 0.f));

  glm::mat4 viewMatrix = glm::lookAt(
      glm::vec3(0.f, 0.2f, 0.8f),
      glm::vec3(
          globalModel->global_matrix[3][0],
          globalModel->global_matrix[3][1],
          globalModel->global_matrix[3][2]
      ),
      glm::vec3(0.f, 1.f, 0.f));
  viewMatrix = glm::scale(viewMatrix, glm::vec3(0.2, 0.2, 0.2));

  glm::mat4 projectionMatrix = glm::perspective(45.f, (float)SCREEN_W/(float)SCREEN_H, 0.001f, 4.f);

  globalModel->render(viewMatrix, projectionMatrix);

  SDL_GL_SwapWindow(win.get());
}
