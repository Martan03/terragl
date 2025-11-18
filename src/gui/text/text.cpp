#include "text.hpp"

#include "glad/gl.h"

namespace tgl::gui {

Text::Text(
    glm::vec2 pos,
    glm::vec2 size,
    TextSystem &sys,
    std::string text,
    glm::vec3 color
) :
    Widget(pos, size),
    _sys(sys),
    _vbo(GL_ARRAY_BUFFER),
    _text(text),
    _color(color) {
    init_buffers();
    compile();
}

void Text::render() {
    _vao.bind();
    _sys.font()._atlas.bind();

    _sys.program().use();
    glUniform3f(_sys.col_loc(), _color.r, _color.g, _color.b);

    glDrawArrays(GL_TRIANGLES, 0, _text.size() * 6);
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
    vert.reserve(_text.size() * 6 * 4);
    float xpos = _pos.x;
    for (char c : _text) {
        const auto &ch = _sys.font()._chars[c];
        float x = xpos + ch.bearing.x;
        float y = _pos.y - ch.bearing.y;

        float u1 = ch.uv.x + ch.uv_size.x;
        float v1 = ch.uv.y + ch.uv_size.y;
        vert.insert(vert.end(), { x, y, ch.uv.x, ch.uv.y });
        vert.insert(vert.end(), { x, y + ch.size.y, ch.uv.x, v1 });
        vert.insert(vert.end(), { x + ch.size.x, y, u1, ch.uv.y });
        vert.insert(vert.end(), { x + ch.size.x, y, u1, ch.uv.y });
        vert.insert(vert.end(), { x, y + ch.size.y, ch.uv.x, v1 });
        vert.insert(vert.end(), { x + ch.size.x, y + ch.size.y, u1, v1 });
        xpos += (ch.advance >> 6);
    }

    _vbo.bind();
    _vbo.set(vert);
}

} // namespace tgl::gui
