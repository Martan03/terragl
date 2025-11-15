#pragma once

#include "../../gl/buffer.hpp"
#include "../../gl/program.hpp"
#include "../../gl/vertex_array.hpp"
#include "../widget.hpp"
#include "font.hpp"
#include "system.hpp"

#include <glm/glm.hpp>

namespace tgl::gui {

class Text : public Widget {
public:
    Text(
        glm::vec2 pos,
        glm::vec2 size,
        TextSystem &system,
        std::string text = "",
        glm::vec3 color = glm::vec3(1, 1, 1)
    );

    void render() override;

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
    TextSystem &_sys;
    gl::VertexArray _vao;
    gl::Buffer _vbo;

    std::string _text;
    glm::vec3 _color;
    float _x;
    float _y;

    void init_buffers();
};

} // namespace tgl::gui
