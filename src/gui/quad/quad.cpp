#include "quad.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace tgl::gui {

static constexpr char VERT_SHADER[]{
#embed "shader.vert" suffix(, )
    0
};

static constexpr char FRAG_SHADER[]{
#embed "shader.frag" suffix(, )
    0
};

Quad::Quad(glm::vec2 pos, glm::vec2 size) :
    Widget(pos, size),
    _program(VERT_SHADER, FRAG_SHADER),
    _vbo(GL_ARRAY_BUFFER) {
    init_buffers();
    set_color(0, 0, 0);
}

void Quad::render() {
    _program.use();
    _vao.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Quad::set_proj(glm::mat4 &proj) {
    _program.use();
    auto proj_loc = _program.uniform_loc("proj");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj));
}

void Quad::set_color(glm::vec3 color) {
    _color = color;
    _program.use();
    auto col_loc = _program.uniform_loc("color");
    glUniform3f(col_loc, _color.r, _color.g, _color.b);
}

void Quad::set_color(float r, float g, float b) {
    set_color(glm::vec3(r, g, b));
}

void Quad::init_buffers() {
    _vao.bind();
    _vbo.bind();

    // clang-format off
    float vertices[] = {
        _pos.x, _pos.y, // TL
        _pos.x, _pos.y + _size.y, // BL
        _pos.x + _size.x, _pos.y, // TR
        _pos.x + _size.x, _pos.y + _size.y // BR
    };
    // clang-format on
    _vbo.set(vertices);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
}

} // namespace tgl::gui
