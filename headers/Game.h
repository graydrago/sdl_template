#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <glm/glm.hpp>

#include "../headers/Control.h"
#include "../headers/Line.h"
#include "../headers/Light.h"
#include "../headers/SegmentCollider.h"

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
        int m_window_screen_width;
        int m_window_screen_height;
        int m_fullscreen_screen_width;
        int m_fullscreen_screen_height;
        float m_fov_angle;
        float m_near_plane;
        float m_far_plane;

        SegmentCollider m_aim_ray;
        glm::mat4 m_view_matrix;
        glm::mat4 m_projection_matrix;

        Control control;
        std::vector<std::shared_ptr<Object>> scene_list;
        //std::map<std::string, std::vector<std::shared_ptr<Object>>> scene_groups;
        
        std::map<std::string, Object*> m_bag;
        std::shared_ptr<Camera> m_camera;
        std::shared_ptr<Light> m_light;
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

        std::shared_ptr<Light> light() { return m_light; }
        std::shared_ptr<Camera> camera() { return m_camera; }

        void fpsControlCamera(Object& _c, float);
        void freeControlCamera(Object& _c, float);
        void toggleFullscreenVideoMode();
        float aspectRatio() { return (float)m_screen_width/(float)m_screen_height; };

        std::shared_ptr<ShaderProgram> shader(std::string name) { return m_shaders_list.at(name); };
        std::shared_ptr<Mesh> mesh(std::string name) { return m_mesh_list.at(name); };
        SegmentCollider aimRay() { return m_aim_ray; }

        void cache(std::string name, std::shared_ptr<ShaderProgram> shader) { m_shaders_list[name] = shader; };
        void cache(std::string name, std::shared_ptr<Mesh> mesh) { m_mesh_list[name] = mesh; };
        void bag(std::string name, Object* v);
        Object* bag(std::string name);

        void operator=(Game const&) = delete;

};
