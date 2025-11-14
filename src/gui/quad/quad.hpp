#pragma once

#include "../../gl/buffer.hpp"
#include "../../gl/program.hpp"
#include "../../gl/vertex_array.hpp"
#include "../widget.hpp"

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

namespace tgl::gui {

class Quad : public Widget {
public:
    Quad(glm::vec2 pos, glm::vec2 size);

    void render() override;

    void set_proj(glm::mat4 &proj) override;

    void set_color(glm::vec3 color);
    void set_color(float r, float g, float b);

private:
    gl::Program _program;
    gl::VertexArray _vao;
    gl::Buffer _vbo;

    glm::vec3 _color;

    void init_buffers();
};

} // namespace tgl::gui
