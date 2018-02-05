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
      std::vector<GLuint> texture_ids;
      GLuint points_id;
      GLuint indeces_points_id;
      GLuint vao;
      ShaderProgram shader;
      int amount_of_points = 500000;
      float point_size = 300.0;

    public:
        using Sample::Sample;

        virtual ~Sample2() {
            glDeleteTextures(texture_ids.size(), texture_ids.data());
            glDeleteBuffers(1, &points_id);
            glDeleteBuffers(1, &indeces_points_id);
        }


        virtual void init() {
            // Init
            game.m_camera_min_speed = 0.5;
            game.m_camera_max_speed = 1.0;

            GLint max_texture_units;
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_units);
            SDL_Log("Max texture image units: %i", max_texture_units);
            max_texture_units = glm::clamp(max_texture_units, 1, 18);

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
                    shader.compile("./assets/glsl/sample2/shader_es.vert", GL_VERTEX_SHADER);
                    shader.compile(prepareFragmentShader(max_texture_units), GL_FRAGMENT_SHADER, false);
                #else
                    shader.compile("./assets/glsl/sample2/shader.vert", GL_VERTEX_SHADER);
                    shader.compile(prepareFragmentShader(max_texture_units), GL_FRAGMENT_SHADER, false);
                #endif
                shader.link();
                shader.use();
            }

            // Create points cloud
            std::random_device rd;
            std::uniform_real_distribution<double> random(-200.0, 200.0);
            std::uniform_real_distribution<double> random_h(0.5, 100.0);
            std::uniform_int_distribution<int> random_i(0, max_texture_units - 1);
            std::srand(std::chrono::system_clock::now().time_since_epoch().count());
            std::vector<glm::vec3> points;
            std::vector<GLint> indeces_points;
            points.reserve(amount_of_points);
            indeces_points.reserve(amount_of_points);
            for (int i = 0; i < amount_of_points; ++i) {
                points.push_back({random(rd), glm::abs(random_h(rd)), random(rd)});
                indeces_points.push_back(random_i(rd));
            }
            std::sort(points.begin(), points.end(), [](glm::vec3 &a, glm::vec3 &b){
                return a.y > b.y;
            });

            //glCreateBuffers(1, &pointes_id); // saved for OpenGL 4.5
            glEnableVertexAttribArray(0);
            glGenBuffers(1, &points_id);
            glBindBuffer(GL_ARRAY_BUFFER, points_id);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_STATIC_DRAW);

            glEnableVertexAttribArray(1);
            glGenBuffers(1, &indeces_points_id);
            glBindBuffer(GL_ARRAY_BUFFER, indeces_points_id);
            glVertexAttribIPointer(1, 1, GL_INT, 0, nullptr);
            glBufferData(GL_ARRAY_BUFFER, indeces_points.size() * sizeof(GLint), indeces_points.data(), GL_STATIC_DRAW);

            if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
                SDL_Log("Init SDL_image: %s", IMG_GetError());
                return;
            }

            // Texture handling
            SDL_Surface* surface;
            std::vector<std::string> file_list;
            int files_limit = glm::clamp(max_texture_units, 8, 19);
            for (int i = 1; i < files_limit; ++i) {
                file_list.push_back("./assets/textures/planet" + std::to_string(i) + ".png");
            }
            texture_ids.resize(file_list.size());

            glGenTextures(texture_ids.size(), texture_ids.data());
            for (int i = 0; i < static_cast<int>(file_list.size()); ++i) {
                surface = load_surface(file_list[i]);
                glActiveTexture(GL_TEXTURE0+i);
                glBindTexture(GL_TEXTURE_2D, texture_ids[i]);
                glEnable(GL_POINT_SPRITE);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                SDL_FreeSurface(surface);
            }
        };


        SDL_Surface* load_surface(std::string file_name) {
            SDL_Surface* surface = IMG_Load(file_name.c_str());
            if (surface == nullptr) {
                SDL_Log("Can't load file: %s", file_name.c_str());
                SDL_Log("Verbose: %s", IMG_GetError());
                return nullptr;
            }

            return surface;
        }


        virtual void update(float elapsed_seconds) {
            shader.setUniform("uPVM", game.m_projection_matrix * game.m_view_matrix);
            shader.setUniform("uCameraPosition", game.aimRay().startPoint());
            shader.setUniform("uPointSize", point_size);
            int size = texture_ids.size();
            for (int i = 0; i < size; ++i) {
                shader.setUniform("uTex" + std::to_string(i), i);
            }
            shader.setUniform("uMaxTextureUnits", static_cast<int>(texture_ids.size()));
            shader.setUniform("uTime", elapsed_seconds);
        };


        virtual void render() {
            glDrawArrays(GL_POINTS, 0, amount_of_points);
        };


        std::string prepareFragmentShader(const int max_texture_units) {
            std::string text;

            #ifdef __EMSCRIPTEN__
            text += "#version 300 es\n"
                    "precision highp int;\n"
                    "precision highp float;\n";
            #else
            text += "#version 430\n";
            #endif
            text += "flat in int vPlanetID;\n"
                    "out vec4 fFragColor;\n";
            for (int i = 0; i < max_texture_units; ++i) {
                text += "uniform sampler2D uTex" + std::to_string(i) + ";\n";
            }

            text += "void main () {\n"
                    "vec4 t;\n";

            int i;
            for (i = 0; i < (max_texture_units - 1); ++i) {
              text += "if (vPlanetID == " + std::to_string(i) + ") {\n"
                          "t = texture(uTex" + std::to_string(i) + ", gl_PointCoord);\n"
                      "} else ";
            }
            text += "{ t = texture(uTex" + std::to_string(i) + ", gl_PointCoord); }\n";
            text += "if (t.a < 0.0001) { discard; }\n"
                    "fFragColor = t;\n"
                    "}";

            return text;
        }
};
