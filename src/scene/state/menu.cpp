#include "menu.hpp"

#include "../scene.hpp"

#if true
#include <GLFW/glfw3.h>
#endif

namespace tgl::scene::state {

Menu::Menu(Scene &scene) :
    State(scene), _quad(glm::vec2(5, 5), glm::vec2(200, 200)) { }

void Menu::render() {
    _scene.state(StateType::Game)->render();
    _quad.render();
}

void Menu::resize() {
    auto proj = _scene.window().ortho();
    _quad.set_proj(proj);
}

void Menu::handle_key(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        _scene.set_state(StateType::Game);
    }
}

} // namespace tgl::scene::state
