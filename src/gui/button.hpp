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
    Button(glm::vec2 pos, glm::vec2 size, TextSystem &sys, std::string text);

    void render() override;

    void set_proj(glm::mat4 &proj) override;

    void fg(glm::vec3 color) { _text.set_fg(color); }
    void bg(glm::vec3 color) { _bg.set_color(color); }

private:
    Quad _bg;
    Text _text;
};

} // namespace tgl::gui
