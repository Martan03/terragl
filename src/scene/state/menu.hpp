#pragma once

#include "state.hpp"

namespace tgl::scene::state {

class Menu : public State {
public:
    Menu(Scene &scene);

    void render() override;
    
    void handle_key(int key, int scancode, int action, int mods) override;
};

} // namespace tgl::scene::state
