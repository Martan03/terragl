#include "button.hpp"

#include <glad/gl.h>

#if true
#include <GLFW/glfw3.h>
#endif

namespace tgl::gui {

Button::Button(glm::vec2 size, TextSystem &sys, std::string text) :
    Widget(glm::vec2(0, 0), size),
    _text(glm::vec2(0, size.y), size, sys, text),
    _bg(glm::vec2(0, 0), size) {
    bg(glm::vec3(0, 0, 0));
}

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

void Button::set_pos(glm::vec2 pos) {
    _pos = pos;
    _bg.set_pos(pos);
    _text.set_pos(glm::vec2(pos.x + _padding.w, pos.y + _size.y - _padding.z));
}

void Button::set_size(glm::vec2 size) {
    _size = size;
    _bg.set_size(size);
    auto tsize = glm::vec2(
        size.x - _padding.y - _padding.w, size.y - _padding.x - _padding.z
    );
    _text.set_size(tsize);
}

void Button::padding(glm::vec4 pad) {
    if (_padding == pad)
        return;
    _padding = pad;
    _text.set_pos(glm::vec2(_pos.x + pad.w, _pos.y + _size.y - pad.z));
}

void Button::exec_on_click() {
    if (_on_click)
        _on_click();
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
