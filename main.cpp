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
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./headers/Mesh.h"
#include "./headers/Model.h"
#include "./headers/Normals.h"
#include "./headers/Control.h"
#include "./headers/Camera.h"
#include "./headers/Object.h"

const int SCREEN_W {800};
const int SCREEN_H {600};

std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> win {
  nullptr, [](SDL_Window* p) { SDL_DestroyWindow(p); SDL_Quit(); }
};
std::unique_ptr<Model> globalModel {nullptr};
std::unique_ptr<Object> root {new Object()};
bool play = true;
Uint32 last_frame_time{};
Control control{};
std::shared_ptr<Camera> camera(new Camera());

void fpsControlCamera(Object &_c, float) {
    Camera &c = dynamic_cast<Camera &>(_c);

    if (control.left) {
        auto left = glm::normalize(glm::cross(c.getUp(), c.getForward()));
        c.setPosition(c.getPosition() + left * 0.05f);
    }
    if (control.right) {
        auto right = glm::normalize(glm::cross(c.getForward(), c.getUp()));
        c.setPosition(c.getPosition() + right * 0.05f);
    }
    if (control.up) {
        c.setPosition(c.getPosition() + glm::normalize(c.getForward()) * 0.05f);
    }
    if (control.down) {
        c.setPosition(c.getPosition() - glm::normalize(c.getForward()) * 0.05f);
    }

    if (control.xrel != 0) { c.setYaw(c.getYaw() + control.xrel / 2); }
    if (control.yrel != 0) { c.setPitch(c.getPitch() + control.yrel / 2); }
};

void freeControlCamera(Object &_c, float) {
    Camera &c = dynamic_cast<Camera &>(_c);

    if (control.left) {
        auto left = glm::normalize(glm::cross(c.getUp(), c.getForward()));
        c.setPosition(c.getPosition() + left * 0.05f);
    }
    if (control.right) {
        auto right = glm::normalize(glm::cross(c.getForward(), c.getUp()));
        c.setPosition(c.getPosition() + right * 0.05f);
    }
    if (control.up) {
        auto pos = c.getPosition();
        pos.y += 0.05;
        c.setPosition(pos);
    }
    if (control.down) {
        auto pos = c.getPosition();
        pos.y -= 0.05;
        c.setPosition(pos);
    }

    if (control.xrel != 0) { c.setYaw(c.getYaw() + control.xrel / 2); }
    if (control.yrel != 0) { c.setPitch(c.getPitch() + control.yrel / 2); }
};

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
    SDL_SetRelativeMouseMode(SDL_TRUE);

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
    
    std::srand(std::chrono::system_clock::now().time_since_epoch().count());


    std::shared_ptr<Mesh> mesh(new Mesh());
    mesh->load("./assets/models/monkey.obj");

    camera->setUpdateCb(fpsControlCamera);
    root->addChild(camera);

    for (int i = -3; i <= 3; i++) {
        for (int j = -3; j <= 3; j++) {
            for (int k = -3; k <= 3; k++) {
                std::shared_ptr<Model> cube{new Model()};
                cube->setMesh(mesh);
                cube->setShader(shaderProgram);
                glm::mat4 m(1.f);
                m = glm::scale(m, glm::vec3(0.2, 0.2, 0.2));
                m = glm::translate(m, glm::vec3(i, j, k));
                cube->setColor(glm::vec3(
                    std::rand() % 255 * (1.f/255.f),
                    std::rand() % 255 * (1.f/255.f),
                    std::rand() % 255 * (1.f/255.f)
                ));
                cube->setMatrix(m);
                cube->setUpdateCb([](Object &m, float elapsed) -> void {
                    m.setMatrix(
                        m.getMatrix() * glm::rotate(glm::mat4(1.f), elapsed * 3.14f, glm::vec3(0.f, 1.f, 0.f))
                    );
                });
                root->addChild(cube);
            }
        }
    }

    last_frame_time = SDL_GetTicks();
    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
    #else
    while (control.play) {
        loop();
    }
    #endif
}


void loop() {
  SDL_Event event;
  control.xrel = 0;
  control.yrel = 0;
  
  while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
          play = false;
      }

      if (event.type == SDL_KEYDOWN) {
          switch (event.key.keysym.sym) {
              case SDLK_a:
                  control.left = true;
                  break;
              case SDLK_d:
                  control.right = true;
                  break;
              case SDLK_w:
                  control.up = true;
                  break;
              case SDLK_s:
                  control.down = true;
                  break;
              case SDLK_ESCAPE:
              case SDLK_q:
                  control.play = false;
                  break;
              case SDLK_1:
                  camera->setUpdateCb(fpsControlCamera);
                  break;
              case SDLK_2:
                  camera->setUpdateCb(freeControlCamera);
                  break;
          }
      } else if (event.type == SDL_KEYUP) {
          switch (event.key.keysym.sym) {
              case SDLK_a:
                  control.left = false;
                  break;
              case SDLK_d:
                  control.right = false;
                  break;
              case SDLK_w:
                  control.up = false;
                  break;
              case SDLK_s:
                  control.down = false;
                  break;
          }
      } else if (event.type == SDL_MOUSEWHEEL) {
          control.zoom += event.wheel.y > 0 ? 1 : -1;
          if (control.zoom < control.zoom_min) {
              control.zoom = control.zoom_min;
          } else if (control.zoom > control.zoom_max) {
              control.zoom = control.zoom_max;
          }
      } else if (event.type == SDL_MOUSEMOTION) {
          control.xrel = event.motion.xrel;
          control.yrel = event.motion.yrel;
      }
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  Uint32 elapsed_since_start_program = SDL_GetTicks();
  Uint32 elapsed_since_last_frame = elapsed_since_start_program - last_frame_time;
  float elapsed_seconds = (float) elapsed_since_last_frame * 0.001;
  last_frame_time = elapsed_since_start_program;


  glm::mat4 viewMatrix = camera->eye();
  viewMatrix = glm::scale(viewMatrix, glm::vec3(0.3, 0.3, 0.3));
  viewMatrix = glm::translate(viewMatrix, glm::vec3(0.f, 0.f, control.zoom/10.f));

  glm::mat4 projectionMatrix = glm::perspective(45.f, (float)SCREEN_W/(float)SCREEN_H, 0.001f, 4.f);

  root->update(elapsed_seconds, viewMatrix);
  root->render(projectionMatrix);

  SDL_GL_SwapWindow(win.get());
}
