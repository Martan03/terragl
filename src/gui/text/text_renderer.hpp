#pragma once

#include "../../gl/program.hpp"
#include "../../gl/window.hpp"
#include "text.hpp"

namespace tgl::gui {

class TextRenderer {
public:
    TextRenderer();

    Font &font() { return _font; }

    void render(gl::Window &win, Text &text);

private:
    gl::Program _program;

    Font _font;
};

} // namespace tgl::gui
