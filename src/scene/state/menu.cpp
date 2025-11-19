#include "menu.hpp"

#include "../scene.hpp"

#if true
#include <GLFW/glfw3.h>
#endif

namespace tgl::scene::state {

Menu::Menu(Scene &scene) : State(scene) {
    auto btn_size = glm::vec2(100, 25);
    auto resume =
        gui::Button(glm::vec2(5, 100), btn_size, _scene.text_sys(), "Resume");
    resume.set_on_click([this]() { _scene.set_state(StateType::Game); });
    _buttons.push_back(std::move(resume));

    auto quit =
        gui::Button(glm::vec2(5, 135), btn_size, _scene.text_sys(), "Quit");
    quit.set_on_click([this]() {
        glfwSetWindowShouldClose(_scene.window().get(), true);
    });
    _buttons.push_back(std::move(quit));
}

void Menu::render() {
    _scene.state(StateType::Game)->render();
    for (auto &button : _buttons) {
        button.render();
    }
}

void Menu::resize() {
    auto proj = _scene.window().ortho();
    for (auto &button : _buttons) {
        button.set_proj(proj);
    }
}

void Menu::handle_key(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        _scene.set_state(StateType::Game);
    }
}

void Menu::handle_click(int button, int action, int mods) {
    // TODO: probably iterate from back
    for (auto &btn : _buttons) {
        btn.on_mouse_click(button, action, _scene.mouse_x(), _scene.mouse_y());
    }
}

} // namespace tgl::scene::state
