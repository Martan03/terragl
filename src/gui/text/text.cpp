#include "text.hpp"

#include "glad/gl.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace tgl::gui {

Text::Text(TextSystem &system, std::string text, glm::vec3 color) :
    Text(glm::vec2(0, 0), glm::vec2(-1, -1), system, text, color) { }

Text::Text(
    glm::vec2 pos, TextSystem &system, std::string text, glm::vec3 color
) :
    Text(pos, glm::vec2(-1, -1), system, text, color) { }

Text::Text(
    glm::vec2 pos,
    glm::vec2 size,
    TextSystem &sys,
    std::string text,
    glm::vec3 color
) :
    Widget(pos, size),
    _sys(&sys),
    _vbo(GL_ARRAY_BUFFER),
    _ebo(GL_ELEMENT_ARRAY_BUFFER),
    _text(text),
    _color(color) {
    init_buffers();
    compile();

    _model = glm::mat4(1);
    _model = glm::translate(_model, glm::vec3(_pos.x, _pos.y, 0));
}

void Text::render() {
    _vao.bind();
    _sys->font()._atlas.bind();

    _sys->program().use();
    glUniform3f(_sys->col_loc(), _color.r, _color.g, _color.b);
    glUniformMatrix4fv(_sys->pos_loc(), 1, GL_FALSE, glm::value_ptr(_model));

    glDrawElements(GL_TRIANGLES, _indices, GL_UNSIGNED_INT, 0);
}

void Text::set_pos(glm::vec2 pos) {
    _pos = pos;
    _model = model();
}

void Text::set_text(std::string text) {
    if (text == _text)
        return;
    _text = text;
    compile();
}

void Text::init_buffers() {
    _vao.bind();
    _vbo.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
}

void Text::compile() {
    std::vector<float> vert;
    std::vector<unsigned> indices;

    vert.reserve(_text.size() * 4);
    indices.reserve(_text.size() * 6);

    float xpos = 0;
    float ypos = 0;
    unsigned vert_cnt = 0;
    for (char c : _text) {
        const auto &ch = _sys->font()._chars[c];
        float x = xpos + ch.bearing.x;
        float y = ypos - ch.bearing.y;

        float u1 = ch.uv.x + ch.uv_size.x;
        float v1 = ch.uv.y + ch.uv_size.y;
        // clang-format off
        vert.insert(vert.end(), {
            x, y, ch.uv.x, ch.uv.y,
            x, y + ch.size.y, ch.uv.x, v1,
            x + ch.size.x, y, u1, ch.uv.y,
            x + ch.size.x, y + ch.size.y, u1, v1
        });
        indices.insert(indices.end(), {
            vert_cnt, vert_cnt + 1, vert_cnt + 2,
            vert_cnt + 2, vert_cnt + 1, vert_cnt + 3
        });
        // clang-format on
        xpos += (ch.advance >> 6);
        vert_cnt += 4;
    }

    _vbo.bind();
    _vbo.set(vert);

    _indices = indices.size();
    _ebo.bind();
    _ebo.set(indices);
}

} // namespace tgl::gui
