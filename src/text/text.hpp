#pragma once

#include "../gl/buffer.hpp"
#include "../gl/program.hpp"
#include "../gl/vertex_array.hpp"
#include "font.hpp"

#include <glm/glm.hpp>

namespace tgl::text {

class Text {
public:
    Text(std::string text, float x, float y);

    void compile(Font &font);

    void render(gl::Program &program, Font &font);

private:
    gl::VertexArray _vao;
    gl::Buffer _vbo;

    std::string _text;
    float _x;
    float _y;

    void init_buffers();
};

} // namespace tgl::text
