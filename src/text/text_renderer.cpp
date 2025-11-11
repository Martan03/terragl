#include "text_renderer.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace tgl::text {

static constexpr char VERT_SHADER[]{
#embed "shader.vert" suffix(, )
    0
};

static constexpr char FRAG_SHADER[]{
#embed "shader.frag" suffix(, )
    0
};

TextRenderer::TextRenderer() :
    _program(VERT_SHADER, FRAG_SHADER),
    _font("/mnt/hdd/school/1mit/terragl/build/NotoSansMono-Regular.ttf") { }

void TextRenderer::render(gl::Window &win, Text &text) {
    _program.use();
    auto proj =
        glm::ortho(0.0f, float(win.width()), float(win.height()), 0.0f);
    auto proj_loc = _program.uniform_loc("proj");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj));

    auto color_loc = _program.uniform_loc("color");
    glUniform3f(color_loc, 1, 1, 1);

    text.render(_program, _font);
}

} // namespace tgl::text
