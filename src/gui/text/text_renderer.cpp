#include "text_renderer.hpp"

#include <glm/ext/matrix_clip_space.hpp>
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

TextRenderer::TextRenderer() :
    _program(VERT_SHADER, FRAG_SHADER), _font("./NotoSansMono-Regular.ttf") { }

void TextRenderer::render(gl::Window &win, Text &text) {
    _program.use();
    auto proj =
        glm::ortho(0.0f, float(win.width()), float(win.height()), 0.0f);
    auto proj_loc = _program.uniform_loc("proj");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj));

    text.render(_program, _font);
}

} // namespace tgl::gui
