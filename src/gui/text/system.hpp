#pragma once

#include "../../gl/program.hpp"
#include "font.hpp"

namespace tgl::gui {

class TextSystem {
public:
    TextSystem();

    void set_proj(glm::mat4 &proj);

    gl::Program &program() { return _program; }
    GLint proj_loc() { return _proj_loc; }
    GLint col_loc() { return _col_loc; }

    Font &font() { return _font; }

private:
    gl::Program _program;
    GLint _proj_loc;
    GLint _col_loc;

    Font _font;
};

} // namespace tgl::gui
