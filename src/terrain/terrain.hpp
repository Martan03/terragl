#pragma once

#include "../gl/buffer.hpp"
#include "../gl/program.hpp"
#include "../gl/vertex_array.hpp"

#include <glm/glm.hpp>

namespace tgl::terrain {

class Terrain {
public:
    Terrain(int width, int height);

    void render(glm::mat4 view, glm::mat4 proj);

private:
    gl::Program _program;
    gl::VertexArray _vao;
    gl::Buffer _vbo;
    gl::Buffer _ebo;

    unsigned int _triangle_cnt = 0;

    void init_buffers(int width, int height);
    void vertex_attrib();
    void set_static_uniforms();
};

} // namespace tgl::terrain
