#pragma once
#define GL_GLEXT_PROTOTYPES
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

class Sample3 : public Sample {
    private:
        GLuint vao_ship;
        GLuint ship_id;
        GLuint texture_effect_id;
        GLuint fb_effects_id;
        GLuint rb_effects_id;
        float pos_x = 0;
        float pos_y = 0;

        GLuint vao_quad;
        GLuint quad_id;

        int ship_size;
        ShaderProgram shader_first_pass;
        ShaderProgram shader_final_pass;
        float last_elapsed_seconds = 0;
        float elapsed_seconds = 0;


    public:
        using Sample::Sample;

        virtual ~Sample3() {
            glDeleteTextures(1, &texture_effect_id);
            glDeleteBuffers(1, &quad_id);
            glDeleteBuffers(1, &ship_id);
            glDeleteVertexArrays(1, &vao_ship);
            glDeleteVertexArrays(1, &vao_quad);
            glDeleteFramebuffers(1, &fb_effects_id);
            glDeleteRenderbuffers(1, &rb_effects_id);
        }


        virtual void init() {
            game.m_use_default_eye_matrix = false;
            game.m_use_default_perspective_matrix = false;

            shader_first_pass = ShaderProgram();
            shader_first_pass.compile("./assets/glsl/sample3/shader_es.vert", GL_VERTEX_SHADER);
            shader_first_pass.compile("./assets/glsl/sample3/shader_es.frag", GL_FRAGMENT_SHADER);
            shader_first_pass.link();

            shader_final_pass = ShaderProgram();
            shader_final_pass.compile("./assets/glsl/sample3/shader_final_pass.vert", GL_VERTEX_SHADER);
            shader_final_pass.compile("./assets/glsl/sample3/shader_final_pass.frag", GL_FRAGMENT_SHADER);
            shader_final_pass.link();

            glClearColor(0, 0, 0, 1);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glGenVertexArrays(1, &vao_ship);
            glBindVertexArray(vao_ship);

            std::vector<float> ship = make_ship();

            glGenBuffers(1, &ship_id);
            glBindBuffer(GL_ARRAY_BUFFER, ship_id);
            glBufferData(GL_ARRAY_BUFFER, ship.size() * sizeof(float), ship.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, nullptr);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(2 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // Effect
            glGenFramebuffers(1, &fb_effects_id);
            glBindFramebuffer(GL_FRAMEBUFFER, fb_effects_id);
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
            } else {
                // TODO throw error
            }

            glGenTextures(1, &texture_effect_id);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_effect_id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, game.m_screen_width, game.m_screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_effect_id, 0);

            glGenRenderbuffers(1, &rb_effects_id);
            glBindRenderbuffer(GL_RENDERBUFFER, rb_effects_id);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_EXT, game.m_screen_width, game.m_screen_height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb_effects_id);

            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // TODO quad
            glGenVertexArrays(1, &vao_quad);
            glBindVertexArray(vao_quad);

            std::vector<glm::vec3> quad{
                {-1, -1, 0},
                {-1,  1, 0},
                { 1,  1, 0},
                { 1,  1, 0},
                { 1, -1, 0},
                {-1, -1, 0},
            };

            glGenBuffers(1, &quad_id);
            glBindBuffer(GL_ARRAY_BUFFER, quad_id);
            glBufferData(GL_ARRAY_BUFFER, quad.size() * sizeof(glm::vec3), quad.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glViewport(0, 0, game.m_screen_width, game.m_screen_height);

            gl_check_error();
            SDL_Log("Screen size %dx%d", game.m_screen_width, game.m_screen_height);
        };


        virtual void update(float elapsed_seconds) {
            this->elapsed_seconds = elapsed_seconds;
            float ration = game.aspectRatio();
            float speed = 40.f;
            float step = elapsed_seconds - last_elapsed_seconds;
            last_elapsed_seconds = elapsed_seconds;
            float dimention = 100;
            game.m_projection_matrix = glm::ortho<float>(
                ration * dimention * -0.5 ,
                ration * dimention * 0.5,
                1.0 * dimention * -0.5,
                1.0 * dimention * 0.5,
                -1.0,
                1000.0);

            if (game.control.left) pos_x -= speed * step;
            if (game.control.right) pos_x += speed * step;
            if (game.control.up) pos_y += speed * step;
            if (game.control.down) pos_y -= speed * step;
        };


        virtual void render() {
            glm::mat4 local = glm::translate(glm::mat4(1.0), {pos_x-7.5, pos_y-8.0, 0.0});
            glm::mat4 VPL = game.m_view_matrix * game.m_projection_matrix * local;

            // Render pass
            glBindFramebuffer(GL_FRAMEBUFFER, fb_effects_id);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glBindVertexArray(vao_ship);
            shader_first_pass.use();
            shader_first_pass.setUniform("uVPL", VPL);
            glDrawArrays(GL_TRIANGLES, 0, ship_size);

            // Final pass
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(vao_quad);
            shader_final_pass.use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_effect_id);
            glm::vec3 screen_size{game.m_screen_width, game.m_screen_height, 0.0};
            shader_final_pass.setUniform("uOffset", static_cast<float>((glm::sin(elapsed_seconds)+1.0)*10.0));
            shader_final_pass.setUniform("uScreenSize", screen_size);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glBindVertexArray(vao_ship);
            shader_first_pass.use();
            shader_first_pass.setUniform("uVPL", VPL);
            //glDrawArrays(GL_TRIANGLES, 0, ship_size);
        };


        virtual void resize() {
            glBindFramebuffer(GL_FRAMEBUFFER, fb_effects_id);

            glViewport(0, 0, game.m_screen_width, game.m_screen_height);

            glBindRenderbuffer(GL_RENDERBUFFER, rb_effects_id);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_EXT, game.m_screen_width, game.m_screen_height);

            glBindTexture(GL_TEXTURE_2D, texture_effect_id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, game.m_screen_width, game.m_screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glBindTexture(GL_TEXTURE_2D, 0);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            
        }


        std::vector<float> make_ship() {
            std::vector<std::string> raw_ship{
                "       w       ",
                "       w       ",
                "       w       ",
                "      www      ",
                "      www      ",
                "   r  www  r   ",
                "   r  www  r   ",
                "   w wwwww w   ",
                "r  wbwwrwwbw  r",
                "r  bwwrrrwwb  r",
                "w  wwwrwrwww  w",
                "w wwwwwwwwwww w",
                "wwwwwrwwwrwwwww",
                "www rrwwwrr www",
                "ww  rr w rr  ww",
                "w      w      w",
            };
            std::reverse(raw_ship.begin(), raw_ship.end());

            std::map<char, glm::vec3> pallete{
                {'r', {0.86, 0.21, 0.21}},
                {'b', {0.15, 0.34, 0.86}},
                {'w', {0.86, 0.86, 0.86}},
                {' ', {0, 0, 0}},
            };

            std::vector<float> ship;

            int max_rows = raw_ship.size();
            int max_cols = raw_ship[0].size();
            for (int row = 0; row < max_rows; ++row) {
                for (int col = 0; col < max_cols; ++col) {
                    auto color_code = raw_ship[row][col];
                    if (color_code == ' ') continue;
                    auto color = pallete[color_code];
                    ship.push_back(col+0); ship.push_back(row+0);
                    ship.push_back(color.r); ship.push_back(color.g); ship.push_back(color.b);
                    ship.push_back(col+0); ship.push_back(row+1);
                    ship.push_back(color.r); ship.push_back(color.g); ship.push_back(color.b);
                    ship.push_back(col+1); ship.push_back(row+1);
                    ship.push_back(color.r); ship.push_back(color.g); ship.push_back(color.b);

                    ship.push_back(col+0); ship.push_back(row+0);
                    ship.push_back(color.r); ship.push_back(color.g); ship.push_back(color.b);
                    ship.push_back(col+1); ship.push_back(row+1);
                    ship.push_back(color.r); ship.push_back(color.g); ship.push_back(color.b);
                    ship.push_back(col+1); ship.push_back(row+0);
                    ship.push_back(color.r); ship.push_back(color.g); ship.push_back(color.b);
                    ship_size += 6;
                }
            }

            return ship;
        }
};
