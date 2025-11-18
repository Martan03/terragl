#pragma once

#include <glm/glm.hpp>

namespace tgl::gui {

class Widget {
public:
    Widget(glm::vec2 pos, glm::vec2 size) : _pos(pos), _size(size) { }
    virtual ~Widget() = default;

    virtual void render() = 0;

    virtual void set_proj(glm::mat4 &proj) { };

    virtual bool on_mouse_click(int button, int action, double x, double y) {
        return false;
    }

    bool contains(double x, double y) {
        return _pos.x <= x && x <= _pos.x + _size.x && _pos.y <= y &&
               y <= _pos.y + _size.y;
    }

protected:
    glm::vec2 _pos;
    glm::vec2 _size;
};

} // namespace tgl::gui
