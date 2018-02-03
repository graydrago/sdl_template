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

#include <GL/gl.h>
#include <GL/glext.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#include "../headers/utils.h"

#include "../headers/Game.h"
#include "../headers/Object.h"
#include "../headers/Mesh.h"
#include "../headers/Camera.h"
#include "../headers/Model.h"
#include "../headers/Line.h"
#include "../headers/ShaderProgram.h"
#include "../headers/PlayMusic.h"
#include "../headers/SegmentCollider.h"
#include "../headers/SphereCollider.h"

#include "../samples/Sample1.cpp"
#include "../samples/Sample2.cpp"


Game::Game() {
    m_fullscreen_mode = false;
    m_screen_width = 1024;
    m_screen_height = 768;
    m_fov_angle = 60.f;
    m_near_plane = 0.001f;
    m_far_plane = 1000.f;
    m_camera = std::make_shared<Camera>();
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
    #else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    #endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    //SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    //SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

    #ifdef __EMSCRIPTEN__
    emscripten_get_canvas_element_size(nullptr, &m_screen_width, &m_screen_height);
    #endif

    window = SDL_CreateWindow(
        "Template",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        m_screen_width, m_screen_height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
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
    m_light = std::make_shared<Light>();
    m_light->position({0.0, 10.0, 10.0});

    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_GetWindowDisplayMode(window, &m_window_display_mode);
}


void Game::run() {
    m_camera->updateCb(std::bind(&Game::fpsControlCamera, this, std::placeholders::_1, std::placeholders::_2));
    m_camera->position({0, 0, 0.5});
    scene_list.push_back(m_camera);

    m_sample = std::make_unique<Sample2>(*this);
    m_sample->init();

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
        switch (event.type) {
            case SDL_QUIT:
            {
                control.play = false;
                break;
            }

            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym) {
                    case SDLK_a:
                    {
                        control.left = true;
                        break;
                    }
                    case SDLK_d:
                    {
                        control.right = true;
                        break;
                    }
                    case SDLK_w:
                    {
                        control.up = true;
                        break;
                    }
                    case SDLK_s:
                    {
                        control.down = true;
                        break;
                    }
                    case SDLK_ESCAPE:
                    {
                        SDL_SetRelativeMouseMode(SDL_FALSE);
                        break;
                    }
                    case SDLK_q:
                    {
                        control.play = false;
                        break;
                    }
                    case SDLK_f:
                    {
                        toggleFullscreenVideoMode();
                        break;
                    }
                    case SDLK_LSHIFT:
                    {
                        m_camera->speed(m_camera_max_speed);
                        break;
                    }
                    case SDLK_1:
                    {
                        m_camera->updateCb(std::bind(&Game::fpsControlCamera, this, std::placeholders::_1, std::placeholders::_2));
                        break;
                    }
                    case SDLK_2:
                    {
                        m_camera->updateCb(std::bind(&Game::freeControlCamera, this, std::placeholders::_1, std::placeholders::_2));
                        break;
                    }
                }
                break;
            }

            case SDL_KEYUP:
            {
                switch (event.key.keysym.sym) {
                    case SDLK_a:
                    {
                        control.left = false;
                        break;
                    }
                    case SDLK_d:
                    {
                        control.right = false;
                        break;
                    }
                    case SDLK_w:
                    {
                        control.up = false;
                        break;
                    }
                    case SDLK_s:
                    {
                        control.down = false;
                        break;
                    }
                    case SDLK_LSHIFT:
                    {
                        m_camera->speed(m_camera_min_speed);
                        break;
                    }
                }
                if (event.key.keysym.mod == KMOD_SHIFT) { m_camera->speed(m_camera_max_speed); }
                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            {
                control.fire = true;
                if (!SDL_GetRelativeMouseMode()) { SDL_SetRelativeMouseMode(SDL_TRUE); }
                #ifdef __EMSCRIPTEN__
                emscripten_request_pointerlock("#canvas", EM_TRUE);
                #endif

                //auto ray_start = aimRay().startPoint();
                //auto ray_end = aimRay().endPoint();

                //auto mesh = std::make_shared<Mesh>();
                //std::vector<float> v{
                    //ray_start.x, ray_start.y, ray_start.z,
                    //ray_end.x, ray_end.y, ray_end.z
                //};
                //mesh->makeVertexBuffer(v);
                //mesh->makeVAO();

                //auto shape = std::make_shared<Line>();
                //shape->mesh(mesh);
                //shape->shader(shader("basic"));
                //shape->color({1.f, 0.f, 0.f});
                //scene_list.push_back(shape);
                break;
            }

            case SDL_MOUSEBUTTONUP:
            {
                control.fire = false;
                break;
            }

            case SDL_MOUSEMOTION:
            {
                control.xrel = event.motion.xrel;
                control.yrel = event.motion.yrel;
                break;
            }

            case SDL_WINDOWEVENT:
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                    {
                        glViewport(0, 0, event.window.data1, event.window.data2);
                    }
                    break;
                }
        }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Uint32 elapsed_since_start_program = SDL_GetTicks();
    Uint32 elapsed_since_last_frame = elapsed_since_start_program - last_frame_time;
    float elapsed_seconds = (float) elapsed_since_last_frame * 0.001;
    last_frame_time = elapsed_since_start_program;


    #ifdef __EMSCRIPTEN__
    {
          double width, height;
          auto r = emscripten_get_element_css_size("canvas-owner", &width, &height);
          if (r == EMSCRIPTEN_RESULT_SUCCESS) {
              if (m_screen_width != static_cast<int>(width) || m_screen_height != static_cast<int>(height)) {
                  m_screen_width = width;
                  m_screen_height = height;
                  SDL_SetWindowSize(window, m_screen_width, m_screen_height);
                  glViewport(0, 0, m_screen_width, m_screen_height);
              }
          }
    }
    #endif

    m_view_matrix = m_camera->eye();
    m_projection_matrix = glm::perspective(
        m_screen_height < m_screen_width ? (float)m_screen_height/(float)m_screen_width : aspectRatio(),
        aspectRatio(),
        m_near_plane,
        m_far_plane);

    auto ray_start =  glm::unProject(
            glm::vec3(m_screen_width/2, m_screen_height/2+10, 0.0),
            m_view_matrix, m_projection_matrix,
            glm::vec4(0.f, 0.f, m_screen_width, m_screen_height));

    auto ray_end =  glm::unProject(
            glm::vec3(m_screen_width/2, m_screen_height/2, 1.0),
            m_view_matrix, m_projection_matrix,
            glm::vec4(0.f, 0.f, m_screen_width, m_screen_height));

    m_aim_ray = SegmentCollider(ray_start, ray_end);
    //m_light_position = glm::vec3(camera->position().x, camera->position().y, camera->position().z);

    for (auto item : scene_list) {
        item->update(elapsed_seconds);
    }
    m_sample->update(elapsed_seconds);

    //auto model_at_gunpoint = bag("neares_model");
    //if (model_at_gunpoint != nullptr) {
        //auto tmp = static_cast<Model*>(model_at_gunpoint);
        //tmp->paint(aimRay(), {0, 1, 0});
        //bag("neares_model", nullptr);
    //}

    for (auto item : scene_list) {
        item->render(m_projection_matrix, m_view_matrix);
    }
    m_sample->render();

    SDL_GL_SwapWindow(window);
}


void Game::fpsControlCamera(Object& _c, float) {
    auto& c = static_cast<Camera&>(_c);

    if (control.left) {
        auto left = glm::normalize(glm::cross(c.up(), c.forward()));
        c.position(c.position() + left * c.speed());
    }
    if (control.right) {
        auto right = glm::normalize(glm::cross(c.forward(), c.up()));
        c.position(c.position() + right * c.speed());
    }
    if (control.up) {
        auto direction = glm::vec3({c.forward().x, 0, c.forward().z});
        auto step = c.position() + glm::normalize(direction) * c.speed();
        c.position(step);
    }
    if (control.down) {
        auto direction = glm::vec3({c.forward().x, 0, c.forward().z});
        auto step = c.position() - glm::normalize(direction) * c.speed();
        c.position(step);
    }

    if (control.xrel != 0) { c.yaw(c.yaw() + control.xrel / 5.0); }
    if (control.yrel != 0) { c.pitch(c.pitch() + control.yrel / 5.0); }
};


void Game::freeControlCamera(Object& _c, float) {
    auto& c = static_cast<Camera&>(_c);

    if (control.left) {
        auto left = glm::normalize(glm::cross(c.up(), c.forward()));
        c.position(c.position() + left * c.speed());
    }
    if (control.right) {
        auto right = glm::normalize(glm::cross(c.forward(), c.up()));
        c.position(c.position() + right * c.speed());
    }
    if (control.up) {
        c.position(c.position() + glm::normalize(c.forward()) * c.speed());
    }
    if (control.down) {
        c.position(c.position() - glm::normalize(c.forward()) * c.speed());
    }

    if (control.xrel != 0) { c.yaw(c.yaw() + control.xrel / 2); }
    if (control.yrel != 0) { c.pitch(c.pitch() + control.yrel / 2); }
};


void Game::toggleFullscreenVideoMode() {
    // TODO fix it
    // program stops responding after returning to windowed mode
    //if (m_fullscreen_mode) {
        //m_fullscreen_mode = false;
        //SDL_SetWindowFullscreen(window, 0);
    //} else {
        //m_fullscreen_mode = true;
        //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
        //m_window_display_mode.h = 1920;
        //m_window_display_mode.h = 1080;
        //SDL_SetWindowDisplayMode(window, &m_window_display_mode);
    //}
}


void Game::bag(std::string name, Object* v) {
    if (v == nullptr) {
        m_bag.erase(name);
    } else {
        m_bag[name] = v;
    }
};


Object* Game::bag(std::string name) {
    auto tmp = m_bag.find(name);
    return tmp == m_bag.end() ? nullptr : tmp->second;
};
