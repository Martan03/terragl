#pragma once

#include "../../gl/buffer.hpp"
#include "../../gl/vertex_array.hpp"
#include "../widget.hpp"
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

    void set_text(std::string text);

private:
    TextSystem &_sys;
    gl::VertexArray _vao;
    gl::Buffer _vbo;

    std::string _text;
    glm::vec3 _color;

    void init_buffers();

    void compile();
};

} // namespace tgl::gui
