#pragma once

#include "../../gui/button.hpp"
#include "state.hpp"

namespace tgl::scene::state {

class Menu : public State {
public:
    Menu(Scene &scene);

    void render() override;

    void resize() override;

    void handle_key(int key, int scancode, int action, int mods) override;

private:
    gui::Button _resume;
};

} // namespace tgl::scene::state
