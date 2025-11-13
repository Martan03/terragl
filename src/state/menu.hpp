#pragma once

#include "state.hpp"

namespace tgl::state {

class Menu : public State {
public:
    Menu(Scene &scene, gl::Window &window);

    void handle_input(float delta) override;

    void render() override;
};

} // namespace tgl::state
