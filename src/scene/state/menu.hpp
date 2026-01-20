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

    void handle_controller(GLFWgamepadstate &state, int jid) override;

private:
    std::vector<gui::Button> _buttons;
    unsigned _active = 0;

    void center_buttons(
        std::function<void(gui::Button &)> btn_action = nullptr
    );

    void set_active(unsigned id);
    void exec_active();
};

} // namespace tgl::scene::state
