#pragma once

#include "../gl/buffer.hpp"
#include "../gl/program.hpp"
#include "../gl/vertex_array.hpp"
#include "font.hpp"

#include <glm/glm.hpp>

namespace tgl::text {

class Text {
public:
    Text(
        std::string text,
        float x,
        float y,
        glm::vec3 color = glm::vec3(1, 1, 1)
    );

    void compile(Font &font);

    void render(gl::Program &program, Font &font);

    void set_text(std::string text) { _text = text; }

private:
    gl::VertexArray _vao;
    gl::Buffer _vbo;

    std::string _text;
    glm::vec3 _color;
    float _x;
    float _y;

    void init_buffers();
};

} // namespace tgl::text
