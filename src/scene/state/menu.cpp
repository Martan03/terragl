#include "menu.hpp"

#include "../scene.hpp"

#if true
#include <GLFW/glfw3.h>
#endif

namespace tgl::scene::state {

Menu::Menu(Scene &scene) :
    State(scene),
    _resume(
        glm::vec2(5, 100), glm::vec2(100, 25), _scene.text_sys(), "Resume"
    ),
    _quit(glm::vec2(5, 135), glm::vec2(100, 25), _scene.text_sys(), "Quit") {
    _resume.set_on_click([this]() { _scene.set_state(StateType::Game); });
    _quit.set_on_click([this]() {
        glfwSetWindowShouldClose(_scene.window().get(), true);
    });
}

void Menu::render() {
    _scene.state(StateType::Game)->render();
    _resume.render();
    _quit.render();
}

void Menu::resize() {
    auto proj = _scene.window().ortho();
    _resume.set_proj(proj);
    _quit.set_proj(proj);
}

void Menu::handle_key(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        _scene.set_state(StateType::Game);
    }
}

void Menu::handle_click(int button, int action, int mods) {
    _resume.on_mouse_click(button, action, _scene.mouse_x(), _scene.mouse_y());
    _quit.on_mouse_click(button, action, _scene.mouse_x(), _scene.mouse_y());
}

} // namespace tgl::scene::state
