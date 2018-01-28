#pragma once

#include <random>
#include <chrono>

#include "../headers/Sample.h"
#include "../headers/Mesh.h"

class Sample1 : public Sample {

    public:
        using Sample::Sample;
        virtual ~Sample1() {}


        virtual void init() {
            {
                auto shader = std::make_shared<ShaderProgram>();
                shader->compile("./assets/basic.frag", GL_FRAGMENT_SHADER);
                shader->compile("./assets/basic.vert", GL_VERTEX_SHADER);
                shader->link();
                game.cache("basic", shader);
            }
            {
                auto shader = std::make_shared<ShaderProgram>();
                shader->compile("./assets/onePointLight.frag", GL_FRAGMENT_SHADER);
                shader->compile("./assets/onePointLight.vert", GL_VERTEX_SHADER);
                shader->link();
                game.cache("one_point_light", shader);
            }
            {
                auto mesh = std::make_shared<Mesh>();
                std::vector<GLfloat> v(6, 0.f);
                mesh->makeVertexBuffer(v);

                auto shape = std::make_shared<Line>();
                shape->mesh(mesh);
                shape->shader(game.shader("basic"));
                shape->color({1.f, 0.f, 0.f});
                shape->updateCb([](Object &m, float) {
                    auto& line = static_cast<Line&>(m);
                    auto& game = Game::instance();
                    line.changePoints(game.aimRay().startPoint(), game.aimRay().endPoint());
                });
                game.scene_list.push_back(shape);
            }
            {
                std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
                mesh->load("./assets/models/sphere.json");
                //mesh->load("./assets/models/smooth_monkey.json");
                game.cache("monkey", mesh);
            }
            {
                std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
                mesh->load("./assets/models/sphere.json");
                game.cache("sphere", mesh);
            }

            std::random_device rd;
            std::uniform_real_distribution<double> random(0.0, 1.0);
            std::srand(std::chrono::system_clock::now().time_since_epoch().count());

            //PlayMusic music;
            //music.load("./assets/music/The Endless Cycle.ogg");
            //music.play();


            for (int i = -3; i <= 3; i++) {
                for (int j = -3; j <= 3; j++) {
                    for (int k = -3; k <= 3; k++) {
                        auto cube = std::make_shared<Model>();
                        cube->color(glm::vec3(random(rd), random(rd), random(rd)));
                        cube->mesh(game.mesh("monkey"));
                        cube->shader(game.shader("one_point_light"));
                        cube->position({i, j, k});
                        //cube->shininess(glm::abs(i + j * k));
                        cube->scale({0.1f, 0.1f, 0.1f});
                        cube->updateCb([](Object& m, float) {
                            auto& game = Game::instance();
                            if (!game.control.fire) return;

                            auto collider = std::static_pointer_cast<SphereCollider>(m.collider());
                            auto& model = static_cast<Model&>(m);
                            bool has_intersection = testIntersection(game.aimRay(), *collider);
                            if (has_intersection) {
                                auto obj = game.bag("neares_model");
                                if (obj == nullptr) {
                                    game.bag("neares_model", &model);
                                } else {
                                    auto start_point = game.aimRay().startPoint();
                                    auto dist_saved = obj->position() - start_point ;
                                    auto dist_model = model.position() - start_point ;
                                    if (glm::dot(dist_model, dist_model) < glm::dot(dist_saved, dist_saved)) {
                                        game.bag("neares_model", &model);
                                    }
                                }
                            }
                        });

                        auto collider = std::make_shared<SphereCollider>();
                        collider->center({i, j, k});
                        collider->radius(0.1f);
                        cube->collider(collider);
                        game.scene_list.push_back(cube);
                    }
                }
            }
        };


        virtual void update(float /*elapsed_seconds*/) {
        };


        virtual void render() {
        };

};
