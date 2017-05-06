#pragma once

#include <glm/glm.hpp>
#include <functional>

class Camera {
    private:
        std::function<void(Camera&, float)> update_cb;

    public:
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 forward;
        glm::vec3 position;

        float yaw;
        float pitch;

        Camera();
        virtual ~Camera() {};
        glm::mat4 eye();
        void setUpdateCb(std::function<void(Camera&, float)> cb);
        void update(float elapsed_time);
};
