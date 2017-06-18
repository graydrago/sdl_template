/* NOT FINISHED */
#define GLEW_STATIC
#define GLM_ENABLE_EXPERIMENTAL

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
#include <glm/ext.hpp>
#include <glm/gtx/polar_coordinates.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./headers/Mesh.h"
#include "./headers/Model.h"
#include "./headers/Normals.h"
#include "./headers/Control.h"
#include "./headers/Camera.h"
#include "./headers/Object.h"

const int SCREEN_W {1024};
const int SCREEN_H {768};

std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> win {
    nullptr, [](SDL_Window* p) { SDL_DestroyWindow(p); SDL_Quit(); }
};
std::vector<std::shared_ptr<Object>> scene_list{};
bool play = true;
Uint32 last_frame_time{};
Control control{};
std::shared_ptr<Camera> camera(new Camera());
glm::vec3 camera_old_pos{0.f};

void fpsControlCamera(Object &_c, float) {
    float speed = 0.02f;
    Camera &c = dynamic_cast<Camera &>(_c);
    camera_old_pos = c.getPosition();

    if (control.left) {
        auto left = glm::normalize(glm::cross(c.getUp(), c.getForward()));
        c.setPosition(c.getPosition() + left * speed);
    }
    if (control.right) {
        auto right = glm::normalize(glm::cross(c.getForward(), c.getUp()));
        c.setPosition(c.getPosition() + right * speed);
    }
    if (control.up) {
        auto step = c.getPosition() + glm::normalize(c.getForward()) * speed;
        c.setPosition({step.x, c.getPosition().y, step.z});
    }
    if (control.down) {
        auto step = c.getPosition() - glm::normalize(c.getForward()) * speed;
        c.setPosition({step.x, c.getPosition().y, step.z});
    }

    if (control.xrel != 0) { c.setYaw(c.getYaw() + control.xrel / 2); }
    if (control.yrel != 0) { c.setPitch(c.getPitch() + control.yrel / 2); }
};

void freeControlCamera(Object &_c, float) {
    Camera &c = dynamic_cast<Camera &>(_c);
    camera_old_pos = c.getPosition();

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
    
    std::srand(std::chrono::system_clock::now().time_since_epoch().count());


    std::shared_ptr<Mesh> mesh(new Mesh());
    mesh->load("./assets/models/monkey.obj");

    camera->setUpdateCb(fpsControlCamera);
    scene_list.push_back(camera);

    for (int i = -3; i <= 3; i++) {
        for (int j = -3; j <= 3; j++) {
            for (int k = -3; k <= 3; k++) {
                std::shared_ptr<Model> cube{new Model()};
                cube->setMesh(mesh);
                cube->setShader(shaderProgram);
                cube->setColor(glm::vec3(
                    std::rand() % 255 * (1.f/255.f),
                    std::rand() % 255 * (1.f/255.f),
                    std::rand() % 255 * (1.f/255.f)
                ));
                cube->setPosition({i/5.f, j/5.f, k/5.f});
                cube->setScale({0.1f, 0.1f, 0.1f});
                cube->setUpdateCb([](Object &m, float) -> void {
                    auto delta = glm::normalize(camera->getPosition() - m.getPosition());
                    auto polar = glm::polar(delta);
                    m.setRotation({0, polar.y, 0});
                });
                scene_list.push_back(cube);
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

  glm::mat4 projectionMatrix = glm::perspective(
      glm::degrees(((float)SCREEN_H/(float)SCREEN_W) * 1.2f),
      (float)SCREEN_W/(float)SCREEN_H, 0.001f, 10.f);

  for (auto item : scene_list) {
      item->update(elapsed_seconds);
      item->render(projectionMatrix, viewMatrix);
  }

  SDL_GL_SwapWindow(win.get());
}
