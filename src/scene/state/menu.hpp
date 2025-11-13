#pragma once

#include "state.hpp"

namespace tgl::scene::state {

class Menu : public State {
public:
    Menu(Scene &scene);

    void handle_input(float delta) override;

    void render() override;
};

} // namespace tgl::scene::state
