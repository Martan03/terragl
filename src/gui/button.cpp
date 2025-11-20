#include "button.hpp"

#if true
#include <GLFW/glfw3.h>
#endif

namespace tgl::gui {

Button::Button(glm::vec2 size, TextSystem &sys, std::string text) :
    Widget(glm::vec2(0, 0), size),
    _text(glm::vec2(5, size.y - 5), size, sys, text),
    _bg(glm::vec2(0, 0), size) { }

Button::Button(
    glm::vec2 pos, glm::vec2 size, TextSystem &sys, std::string text
) :
    Widget(pos, size),
    _text(glm::vec2(pos.x + 5, pos.y + size.y - 5), size, sys, text),
    _bg(pos, size) { }

void Button::render() {
    _bg.render();
    _text.render();
}

void Button::set_proj(glm::mat4 &proj) {
    // Text projection matrix is set via the text system
    _bg.set_proj(proj);
}

void Button::set_pos(glm::vec2 pos) {
    _pos = pos;
    _bg.set_pos(pos);
    _text.set_pos(glm::vec2(pos.x + 5, pos.y + _size.y - 5));
}

bool Button::on_mouse_click(int button, int action, double x, double y) {
    if (!contains(x, y))
        return false;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (_on_click)
            _on_click();
        return true;
    }
    return false;
}

} // namespace tgl::gui
