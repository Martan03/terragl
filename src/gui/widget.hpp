#pragma once

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace tgl::gui {

class Widget {
public:
    Widget(glm::vec2 pos, glm::vec2 size) : _pos(pos), _size(size) { }
    Widget(Widget &&) = default;
    virtual ~Widget() = default;

    virtual void render() = 0;

    virtual void set_proj(glm::mat4 &proj) { };

    virtual bool on_mouse_click(int button, int action, double x, double y) {
        return false;
    }

    glm::mat4 model() const {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(_pos, 0));
        return model;
    }

    bool contains(double x, double y) {
        return _pos.x <= x && x <= _pos.x + _size.x && _pos.y <= y &&
               y <= _pos.y + _size.y;
    }

    virtual void set_pos(glm::vec2 pos) { _pos = pos; }
    virtual void set_size(glm::vec2 size) { _size = size; }

protected:
    glm::vec2 _pos;
    glm::vec2 _size;
};

} // namespace tgl::gui
