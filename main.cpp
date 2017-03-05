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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./headers/utils.h"
#include "./headers/ShaderProgram.h"
#include "./headers/Model.h"
#include "./headers/Normals.h"

const int SCREEN_W {800};
const int SCREEN_H {600};

SDL_Window* win = nullptr;
ShaderProgram* globalShaderProgram = nullptr;
Model* globalModel = nullptr;
Normals* globalNormal = nullptr;
GLuint vaoHandle;
GLuint indexBufferHandle;
GLuint positonBufferHundle;
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

    win = SDL_CreateWindow("Template",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            SCREEN_W, SCREEN_H,
            SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (win == nullptr) {
        std::cerr << "Unable to create window: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);

    }

    #ifdef TEST_SDL_LOCK_OPTS
    EM_ASM("SDL.defaults.copyOnLock = false; SDL.defaults.discardOnLock = true; SDL.defaults.opaqueFrontBuffer = false;");
    #endif

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

    glEnable(GL_DEPTH_TEST);

    gl_info();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    Model model;
    globalModel = &model;
    model.load("./assets/models/monkey.obj");
    model.printInfo();

    Normals normals(model);
    globalNormal = &normals;

    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);
    positonBufferHundle = vboHandles[0];
    GLuint normalBufferHandle = vboHandles[1];

    glBindBuffer(GL_ARRAY_BUFFER, positonBufferHundle);
    //glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(float), &(model.vertices)[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, model.vertices.size() * sizeof(float), &(model.vertices)[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, model.normals.size() * sizeof(float), &(model.normals)[0], GL_STATIC_DRAW);

    //glGenBuffers(1, &indexBufferHandle);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferHandle);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indeces.size() * sizeof(int), &(model.indeces)[0], GL_STATIC_DRAW);

    ShaderProgram program;
    #ifdef __EMSCRIPTEN__
    program.compile("./assets/es2/es_basic.frag", GL_FRAGMENT_SHADER);
    program.compile("./assets/es2/es_basic.vert", GL_VERTEX_SHADER);
    #else
    program.compile("./assets/onePointLight.frag", GL_FRAGMENT_SHADER);
    program.compile("./assets/onePointLight.vert", GL_VERTEX_SHADER);
    #endif
    program.link();
    program.use();
    globalShaderProgram = &program;

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, positonBufferHundle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
    #else
    while (play) {
        loop();
    }
    #endif

    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(win);
    SDL_Quit();
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

  glm::mat4 modelMatrix = glm::mat4(1.f);
  //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
  modelMatrix = glm::rotate(modelMatrix, glm::radians((float) (elapsed/10 % 360)), glm::vec3(0.f, 1.f, 0.f));

  glm::mat4 viewMatrix = glm::lookAt(
      glm::vec3(0.f, 1.f, 2.f),
      glm::vec3(0.f, 0.f, 0.f),
      glm::vec3(0.f, 1.f, 0.f));

  glm::mat4 projectionMatrix = glm::perspective(45.f, (float)SCREEN_W/(float)SCREEN_H, 0.001f, 4.f);

  glm::mat4 PVM = projectionMatrix * viewMatrix * modelMatrix;

  globalShaderProgram->use();
  globalShaderProgram->setUniform("VM", viewMatrix * modelMatrix);
  globalShaderProgram->setUniform("PVM", PVM);
  globalShaderProgram->setUniform("NM", glm::mat3(glm::transpose(glm::inverse(viewMatrix * modelMatrix))));

  glBindVertexArray(vaoHandle);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferHandle);
  //glDrawElements(GL_TRIANGLES, globalModel->indeces.size(), GL_UNSIGNED_INT, NULL);
  glBindBuffer(GL_ARRAY_BUFFER, positonBufferHundle);
  glDrawArrays(GL_TRIANGLES, 0, globalModel->vertices.size());

  globalNormal->render(PVM);

  SDL_GL_SwapWindow(win);
}
