#pragma once

#include <memory>
#include <vector>

#include "../../gui/quad/quad.hpp"
#include "../../gui/widget.hpp"
#include "state.hpp"

namespace tgl::scene::state {

class Settings : public State {
public:
    Settings(Scene &scene);

    void render() override;

    void resize() override;

    void handle_key(int key, int scancode, int action, int mods) override;

    void handle_click(int button, int action, int mods) override;

private:
    std::vector<std::unique_ptr<gui::Widget>> _widgets;
    gui::Quad _map;

    void layout();
};

} // namespace tgl::scene::state
