#include "quad.hpp"

#include "glad/gl.h"

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

Quad::Quad(glm::vec2 pos, glm::vec2 size) : Quad(pos, size, nullptr) { }

Quad::Quad(gl::Texture *texture) :
    Quad(glm::vec2(0, 0), glm::vec2(0, 0), texture) { }

Quad::Quad(glm::vec2 pos, glm::vec2 size, gl::Texture *texture) :
    Widget(pos, size),
    _program(VERT_SHADER, FRAG_SHADER),
    _vbo(GL_ARRAY_BUFFER),
    _texture(texture) {
    init_buffers();
    set_uniforms();

    _use_tex = texture != nullptr;
    auto tex_loc = _program.uniform_loc("useTex");
    glUniform1i(tex_loc, _use_tex);
    tex_loc = _program.uniform_loc("tex");
    glUniform1i(tex_loc, 0);
}

void Quad::render() {
    _program.use();
    _vao.bind();
    if (_use_tex) {
        _texture->bind();
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Quad::set_proj(glm::mat4 &proj) {
    _program.use();
    auto proj_loc = _program.uniform_loc("proj");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj));
}

void Quad::set_pos(glm::vec2 pos) {
    _program.use();
    _pos = pos;
    auto model_loc = _program.uniform_loc("model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model()));
}

void Quad::set_size(glm::vec2 size) {
    _size = size;
    set_vertices();
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
    set_vertices();

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void *)(2 * sizeof(float))
    );
}

void Quad::set_vertices() {
    _vao.bind();
    _vbo.bind();

    // clang-format off
    float vertices[] = {
        0, 0, 0, 1, // TL
        0, _size.y, 0, 0, // BL
        _size.x, 0, 1, 1, // TR
        _size.x, _size.y, 1, 0, // BR
    };
    // clang-format on
    _vbo.set(vertices);
}

void Quad::set_uniforms() {
    _program.use();
    auto col_loc = _program.uniform_loc("color");
    glUniform3f(col_loc, _color.r, _color.g, _color.b);
    auto model_loc = _program.uniform_loc("model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model()));
    auto tex_loc = _program.uniform_loc("useTex");
    glUniform1i(tex_loc, _use_tex);
}

} // namespace tgl::gui
