#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include <string>
#include <memory>

#include "../headers/Control.h"

class Object;
class Camera;
class Mesh;
class ShaderProgram;

class Game {
    private:
        bool m_fullscreen_mode;
        SDL_DisplayMode m_window_display_mode;

        int m_screen_width;
        int m_screen_height;
        Control control;
        std::vector<std::shared_ptr<Object>> scene_list;
        std::shared_ptr<Camera> camera;
        std::map<std::string, std::shared_ptr<ShaderProgram>> m_shaders_list;
        std::map<std::string, std::shared_ptr<Mesh>> m_mesh_list;

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
        void toggleFullscreenVideoMode();

        std::shared_ptr<ShaderProgram> getShader(std::string name) { return m_shaders_list.at(name); };
        std::shared_ptr<Mesh> getMesh(std::string name) { return m_mesh_list.at(name); };

        void res(std::string name, std::shared_ptr<ShaderProgram> shader) { m_shaders_list[name] = shader; };
        void res(std::string name, std::shared_ptr<Mesh> mesh) { m_mesh_list[name] = mesh; };

        void operator=(Game const&) = delete;

};
