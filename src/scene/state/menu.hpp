#pragma once

#include <vector>

#include "../../gui/button.hpp"
#include "state.hpp"

namespace tgl::scene::state {

class Menu : public State {
public:
    Menu(Scene &scene);

    void render() override;

    void resize() override;

    void handle_key(int key, int scancode, int action, int mods) override;

    void handle_click(int button, int action, int mods) override;

private:
    std::vector<gui::Button> _buttons;
};

} // namespace tgl::scene::state
