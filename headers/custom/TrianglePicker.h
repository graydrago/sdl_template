#pragma once

#include <vector>
#include <GL/glew.h>

#include "../Model.h"

class TrianglePicker : public Model {
    private:
      //std::vector<float> m_colors;
      int m_colors_amount;
      GLuint m_colors_buffer;
      GLuint m_vao;

    public:
        using Model::mesh;

        virtual ~TrianglePicker() {
            if (m_colors_buffer > 0) glDeleteBuffers(1, &m_colors_buffer);
            if (m_vao > 0) glDeleteVertexArrays(1, &m_vao);
        }

        void render(const glm::mat4 &P, const glm::mat4 &V) const noexcept;
        virtual void mesh(std::shared_ptr<Mesh> v) noexcept;
        void changeTriangleColor(int id, glm::vec3 _color);
        void paint(SegmentCollider ray, glm::vec3 _color);
};
