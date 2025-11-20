#pragma once

#include <string>

#include "quad/quad.hpp"
#include "text/system.hpp"
#include "text/text.hpp"
#include "widget.hpp"

#include <glm/glm.hpp>

namespace tgl::gui {

class Button : public Widget {
public:
    Button(glm::vec2 size, TextSystem &sys, std::string text);
    Button(glm::vec2 pos, glm::vec2 size, TextSystem &sys, std::string text);

    void render() override;

    void set_proj(glm::mat4 &proj) override;
    void set_pos(glm::vec2 pos) override;

    void set_on_click(std::function<void()> callback) { _on_click = callback; }
    bool on_mouse_click(int button, int action, double x, double y) override;

    void fg(glm::vec3 color) { _text.set_fg(color); }
    void bg(glm::vec3 color) { _bg.set_color(color); }

    void padding(glm::vec4 pad);
    void padding(glm::vec2 pad) { padding(glm::vec4(pad, pad)); }
    void padding(float pad) { padding(glm::vec4(pad)); }

private:
    Quad _bg;
    Text _text;

    glm::vec4 _padding;

    std::function<void()> _on_click;
};

} // namespace tgl::gui
