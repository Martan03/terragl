#pragma once

#include "../gl/buffer.hpp"
#include "../gl/program.hpp"
#include "../gl/vertex_array.hpp"

#include <glm/glm.hpp>

namespace tgl::sky {

class Sky {
public:
    Sky();

    void render(glm::mat4 view, glm::mat4 proj);

private:
    gl::Program _program;
    gl::VertexArray _vao;
    gl::Buffer _vbo;
    gl::Buffer _ebo;

    void init();
};

} // namespace tgl::sky
