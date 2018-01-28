#pragma once
#define GL_GLEXT_PROTOTYPES


#include <random>
#include <chrono>
#include <vector>
#include <algorithm>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>
#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

#include "../headers/Sample.h"
#include "../headers/Mesh.h"
#include "../headers/Camera.h"

class Sample2 : public Sample {
    private:
      SDL_Surface* surface;
      GLuint texture_id;
      GLuint pointes_id;
      GLuint vao;
      ShaderProgram shader;
      int amount_of_points = 100000;
      float point_size = 300.0;

    public:
        using Sample::Sample;

        virtual ~Sample2() {
            glDeleteTextures(1, &texture_id);
            glDeleteBuffers(1, &pointes_id);
            SDL_FreeSurface(surface);
        }


        virtual void init() {
            // init members
            texture_id = 0;

            glClearColor(0, 0, 0, 1);
            glEnable(GL_BLEND);
            //glEnable(GL_POINT_SMOOTH);
            glEnable(GL_PROGRAM_POINT_SIZE);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            //glCreateVertexArrays(1, &vao); // saved for OpenGL 4.5
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            {
                shader = ShaderProgram();
                #ifdef EMSCRIPTEN
                    shader.compile("./assets/glsl/sample2/shader_es.frag", GL_FRAGMENT_SHADER);
                    shader.compile("./assets/glsl/sample2/shader_es.vert", GL_VERTEX_SHADER);
                #else
                    shader.compile("./assets/glsl/sample2/shader.frag", GL_FRAGMENT_SHADER);
                    shader.compile("./assets/glsl/sample2/shader.vert", GL_VERTEX_SHADER);
                #endif
                shader.link();
                shader.use();
            }

            // create points cloud
            std::random_device rd;
            std::uniform_real_distribution<double> random(-100.0, 100.0);
            std::uniform_real_distribution<double> random_h(0.5, 50.0);
            std::srand(std::chrono::system_clock::now().time_since_epoch().count());
            std::vector<glm::vec3> points;
            for (int i = 0; i < amount_of_points; ++i) {
                points.push_back({random(rd), glm::abs(random_h(rd)), random(rd)});
            }
            std::sort(points.begin(), points.end(), [](glm::vec3 &a, glm::vec3 &b){
                return a.y > b.y;
            });

            //glCreateBuffers(1, &pointes_id); // saved for OpenGL 4.5
            glGenBuffers(1, &pointes_id);
            glBindBuffer(GL_ARRAY_BUFFER, pointes_id);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
            glEnableVertexAttribArray(0);
            glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

            if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
                SDL_Log("Init SDL_image: %s", IMG_GetError());
                return;
            }

            std::string file_name = "./assets/textures/planet2.png";
            surface = IMG_Load(file_name.c_str());
            if (surface == nullptr) {
                SDL_Log("Can't load file: %s", file_name.c_str());
                SDL_Log("Verbose: %s", IMG_GetError());
                return;
            }
            SDL_Log("Bytes per pixel %d", surface->format->BytesPerPixel);
            SDL_Log("Width %d", surface->w);
            SDL_Log("Height %d", surface->h);

            // create GL texture
            glGenTextures(1, &texture_id);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glEnable(GL_POINT_SPRITE);
            //glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            //glPointSize(100);
        };


        virtual void update(float /*elapsed_seconds*/) {
            //shader.setUniform("PVM", game.m_projection_matrix);
            
            shader.setUniform("PVM", game.m_projection_matrix * game.m_view_matrix);
            shader.setUniform("CameraPosition", game.aimRay().startPoint());
            shader.setUniform("PointSize", point_size);
        };


        virtual void render() {
            glDrawArrays(GL_POINTS, 0, amount_of_points);
        };


        //void resize() {
            //#ifdef EMSCRIPTEN
            //int width;
            //int height;
            //int is_fullscreen;
            //emscripten_get_canvas_size(&width, &height, &is_fullscreen);
            //if (is_fullscreen) {
                //emscripten_set_canvas_size(width, height);
                //glViewport(0, 0, width, height);
            //}
            //#endif
        //}
};
