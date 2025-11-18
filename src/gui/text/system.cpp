#include "system.hpp"

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

TextSystem::TextSystem() :
    _program(VERT_SHADER, FRAG_SHADER), _font("./NotoSansMono-Regular.ttf") {
    _proj_loc = _program.uniform_loc("proj");
    _col_loc = _program.uniform_loc("color");
    _pos_loc = _program.uniform_loc("model");
}

void TextSystem::set_proj(glm::mat4 &proj) {
    _program.use();
    glUniformMatrix4fv(_proj_loc, 1, GL_FALSE, glm::value_ptr(proj));
}

} // namespace tgl::gui
