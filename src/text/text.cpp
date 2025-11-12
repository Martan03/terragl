#include "text.hpp"

#include "glad/gl.h"

namespace tgl::text {

static constexpr char VERT_SHADER[]{
#embed "shader.vert" suffix(, )
    0
};

static constexpr char FRAG_SHADER[]{
#embed "shader.frag" suffix(, )
    0
};

Text::Text(std::string text, float x, float y) :
    _vbo(GL_ARRAY_BUFFER), _text(text), _x(x), _y(y) {
    init_buffers();
}

void Text::render(gl::Program &program, Font &font) {
    _vao.bind();
    auto tex_loc = program.uniform_loc("text");
    glUniform1i(tex_loc, 0);
    font._atlas.bind();
    glDrawArrays(GL_TRIANGLES, 0, _text.size() * 6);
}

void Text::compile(Font &font) {
    std::vector<float> vert;
    vert.reserve(_text.size() * 6 * 4);
    float xpos = _x;
    for (char c : _text) {
        const auto &ch = font._chars[c];
        float x = xpos + ch.bearing.x;
        float y = _y - ch.bearing.y;

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

void Text::init_buffers() {
    _vao.bind();
    _vbo.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
}

} // namespace tgl::text
