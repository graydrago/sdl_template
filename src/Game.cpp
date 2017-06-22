#define GLEW_STATIC
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <chrono>
#include <functional>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <memory>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/polar_coordinates.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef __EMSCRIPTEN__
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <emscripten.h>
#include <SDL/SDL_mixer.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#include <SDL2/SDL_mixer.h>
#endif

#include "../headers/utils.h"

#include "../headers/Game.h"
#include "../headers/Object.h"
#include "../headers/Mesh.h"
#include "../headers/Camera.h"
#include "../headers/Model.h"
#include "../headers/ShaderProgram.h"
#include "../headers/PlayMusic.h"


Game::Game() {
    m_screen_width = 1024;
    m_screen_height = 768;
    camera = std::make_shared<Camera>();
}


Game::~Game() {
    Mix_CloseAudio();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        auto err_text = std::string("Unable to init SDL: ") + SDL_GetError();
        throw new std::runtime_error(err_text);
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 4096)) {
        auto err_text = std::string("SDL_Mixer Error: ") + Mix_GetError();
        throw new std::runtime_error(err_text);
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

    window = SDL_CreateWindow(
        "Template",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_screen_width, m_screen_height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        auto err_text = std::string("Unable to create window: ") + SDL_GetError();
        throw new std::runtime_error(err_text);
    }

    #ifdef TEST_SDL_LOCK_OPTS
    EM_ASM("SDL.defaults.copyOnLock = false; SDL.defaults.discardOnLock = true; SDL.defaults.opaqueFrontBuffer = false;");
    #endif

    context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        auto err_text = std::string("Unable to create GL Context: ") + SDL_GetError();
        throw new std::runtime_error(err_text);
    }

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        auto err_text = std::string("Unable to initialize GLEW: ") + reinterpret_cast<const char*>(glewGetErrorString(err));
        throw new std::runtime_error(err_text);
    }
    std::cout << "GLEW has been inited." << std::endl;
    // If there is "Invalid enum", then it's ok.
    // https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library
    gl_check_error();

    glEnable(GL_DEPTH_TEST);
    gl_info();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    SDL_SetRelativeMouseMode(SDL_TRUE);
}


void Game::run() {
    std::shared_ptr<ShaderProgram> shaderProgram(new ShaderProgram());
    #ifdef __EMSCRIPTEN__
    shaderProgram->compile("./assets/es2/es_onePointLight.frag", GL_FRAGMENT_SHADER);
    shaderProgram->compile("./assets/es2/es_onePointLight.vert", GL_VERTEX_SHADER);
    #else
    shaderProgram->compile("./assets/onePointLight.frag", GL_FRAGMENT_SHADER);
    shaderProgram->compile("./assets/onePointLight.vert", GL_VERTEX_SHADER);
    #endif
    shaderProgram->link();
    
    PlayMusic music;
    music.load("./assets/music/The Endless Cycle.ogg");
    music.play();

    std::srand(std::chrono::system_clock::now().time_since_epoch().count());

    std::shared_ptr<Mesh> mesh(new Mesh());
    mesh->load("./assets/models/monkey.obj");

    camera->setUpdateCb(std::bind(&Game::fpsControlCamera, this, std::placeholders::_1, std::placeholders::_2));
    scene_list.push_back(camera);

    for (int i = -5; i <= 5; i++) {
        for (int j = -5; j <= 5; j++) {
            for (int k = -5; k <= 5; k++) {
                std::shared_ptr<Model> cube{new Model()};
                cube->setMesh(mesh);
                cube->setShader(shaderProgram);
                cube->setColor(glm::vec3(
                    std::rand() % 255 * (1.f/255.f),
                    std::rand() % 255 * (1.f/255.f),
                    std::rand() % 255 * (1.f/255.f)
                ));
                cube->setPosition({i, j, k});
                cube->setScale({0.1f, 0.1f, 0.1f});
                cube->setUpdateCb([&](Object &m, float) -> void {
                    auto delta = glm::normalize(camera->getPosition() - m.getPosition());
                    auto polar = glm::polar(delta);
                    m.setRotation({0, polar.y, 0});
                });
                scene_list.push_back(cube);
            }
        }
    }

    #ifdef __EMSCRIPTEN__
      emscripten_set_main_loop([]() { Game::instance().loop(); }, 0, 1);
    #else
      while (control.play) {
          loop();
      }
    #endif
}


void Game::loop() noexcept {
    SDL_Event event;
    control.xrel = 0;
    control.yrel = 0;
    Uint32 last_frame_time{};
  
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            control.play = false;
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
                    camera->setUpdateCb(std::bind(&Game::fpsControlCamera, this, std::placeholders::_1, std::placeholders::_2));
                    break;
                case SDLK_2:
                    camera->setUpdateCb(std::bind(&Game::freeControlCamera, this, std::placeholders::_1, std::placeholders::_2));
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
        glm::degrees(((float)m_screen_height/(float)m_screen_width) * 1.2f),
        (float)m_screen_width/(float)m_screen_height, 0.001f, 10.f);

    for (auto item : scene_list) {
        item->update(elapsed_seconds);
        item->render(projectionMatrix, viewMatrix);
    }

    SDL_GL_SwapWindow(window);
}


void Game::fpsControlCamera(Object &_c, float) {
    float speed = 0.02f;
    Camera &c = dynamic_cast<Camera &>(_c);

    if (control.left) {
        auto left = glm::normalize(glm::cross(c.getUp(), c.getForward()));
        c.setPosition(c.getPosition() + left * speed);
    }
    if (control.right) {
        auto right = glm::normalize(glm::cross(c.getForward(), c.getUp()));
        c.setPosition(c.getPosition() + right * speed);
    }
    if (control.up) {
        auto direction = glm::vec3({c.getForward().x, c.getPosition().y, c.getForward().z});
        auto step = c.getPosition() + glm::normalize(direction) * speed;
        c.setPosition(step);
    }
    if (control.down) {
        auto direction = glm::vec3({c.getForward().x, c.getPosition().y, c.getForward().z});
        auto step = c.getPosition() - glm::normalize(direction) * speed;
        c.setPosition(step);
    }

    if (control.xrel != 0) { c.setYaw(c.getYaw() + control.xrel / 2); }
    if (control.yrel != 0) { c.setPitch(c.getPitch() + control.yrel / 2); }
};


void Game::freeControlCamera(Object &_c, float) {
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
