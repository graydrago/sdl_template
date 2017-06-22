#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <memory>

#include "../headers/Control.h"

class Object;
class Camera;

class Game {
    private:
        int m_screen_width;
        int m_screen_height;
        Control control;
        std::vector<std::shared_ptr<Object>> scene_list;
        std::shared_ptr<Camera> camera;

        SDL_Window *window;
        SDL_GLContext context;

        Game();

    public:
        // have to make a singleton for emscripten_set_main_loop supports
        Game(Game const&) = delete;
        static Game& instance() { static Game instance; return instance; }
        virtual ~Game();

        void init();
        void run();
        void loop() noexcept;

        void fpsControlCamera(Object &_c, float);
        void freeControlCamera(Object &_c, float);

        void operator=(Game const&) = delete;

};
