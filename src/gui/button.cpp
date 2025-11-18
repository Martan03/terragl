#include "button.hpp"

namespace tgl::gui {

Button::Button(
    glm::vec2 pos, glm::vec2 size, TextSystem &sys, std::string text
) :
    Widget(pos, size),
    _text(glm::vec2(pos.x, pos.y + size.y), size, sys, text),
    _bg(pos, size) { }

void Button::render() {
    _bg.render();
    _text.render();
}

void Button::set_proj(glm::mat4 &proj) {
    // Text projection matrix is set via the text system
    _bg.set_proj(proj);
}

} // namespace tgl::gui
