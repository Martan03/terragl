#include "menu.hpp"

#include <glad/gl.h>

#include "../scene.hpp"

#if true
#include <GLFW/glfw3.h>
#endif

namespace tgl::scene::state {

const int BTN_WIDTH = 125;
const int BTN_HEIGHT = 35;

Menu::Menu(Scene &scene) : State(scene) {
    auto btn_size = glm::vec2(BTN_WIDTH, BTN_HEIGHT);
    auto btn_pad = glm::vec2(10, 15);

    auto resume = gui::Button(btn_size, _scene.text_sys(), "Resume");
    resume.padding(btn_pad);
    resume.bg(glm::vec3(0.1, 0.2, 0.2));
    resume.set_on_click([this]() { _scene.set_state(StateType::Game); });
    _buttons.push_back(std::move(resume));

    auto settings = gui::Button(btn_size, _scene.text_sys(), "Settings");
    settings.padding(btn_pad);
    settings.set_on_click([this]() { _scene.set_state(StateType::Settings); });
    _buttons.push_back(std::move(settings));

    auto quit = gui::Button(btn_size, _scene.text_sys(), "Quit");
    quit.padding(btn_pad);
    quit.set_on_click([this]() {
        glfwSetWindowShouldClose(_scene.window().get(), true);
    });
    _buttons.push_back(std::move(quit));

    center_buttons();
}

void Menu::render() {
    _scene.state(StateType::Game)->render();
    for (auto &button : _buttons) {
        button.render();
    }
}

void Menu::resize() {
    auto proj = _scene.window().ortho();
    center_buttons([&proj](gui::Button &btn) { btn.set_proj(proj); });
}

void Menu::handle_key(int key, int scancode, int action, int mods) {
    auto isPressed = [&](int keycode) {
        return key == keycode && action == GLFW_PRESS;
    };

    if (isPressed(GLFW_KEY_ESCAPE)) {
        _scene.set_state(StateType::Game);
    } else if (isPressed(GLFW_KEY_UP) && _active > 0) {
        set_active(_active - 1);
    } else if (isPressed(GLFW_KEY_DOWN) && _active < _buttons.size() - 1) {
        set_active(_active + 1);
    } else if (isPressed(GLFW_KEY_ENTER)) {
        exec_active();
    }
}

void Menu::handle_click(int button, int action, int mods) {
    // TODO: probably iterate from back
    for (auto &btn : _buttons) {
        btn.on_mouse_click(button, action, _scene.mouse_x(), _scene.mouse_y());
    }
}

void Menu::handle_controller(GLFWgamepadstate &state, int jid) {
    auto isPressed = [&](int key) {
        return state.buttons[key] && !_scene.contoller_pressed(jid, key);
    };

    if (isPressed(GLFW_GAMEPAD_BUTTON_START) ||
        isPressed(GLFW_GAMEPAD_BUTTON_B)) {
        _scene.set_state(StateType::Game);
    }
    if (isPressed(GLFW_GAMEPAD_BUTTON_DPAD_UP) && _active > 0)
        set_active(_active - 1);
    if (isPressed(GLFW_GAMEPAD_BUTTON_DPAD_DOWN) &&
        _active < _buttons.size() - 1) {
        set_active(_active + 1);
    }
    if (isPressed(GLFW_GAMEPAD_BUTTON_A))
        exec_active();
}

void Menu::center_buttons(std::function<void(gui::Button &)> btn_action) {
    auto cw = _scene.window().width() / 2;
    auto ch = _scene.window().height() / 2;

    auto sx = cw - BTN_WIDTH / 2;
    auto sy = ch - ((BTN_HEIGHT + 10) * _buttons.size() - 10) / 2;
    for (auto &btn : _buttons) {
        btn.set_pos(glm::vec2(sx, sy));
        sy += BTN_HEIGHT + 10;

        if (btn_action) {
            btn_action(btn);
        }
    }
}

void Menu::set_active(unsigned id) {
    if (_active >= 0 && _active < _buttons.size())
        _buttons[_active].bg(glm::vec3(0, 0, 0));

    _active = id;
    if (_active >= 0 && _active < _buttons.size())
        _buttons[_active].bg(glm::vec3(0.1, 0.2, 0.2));
}

void Menu::exec_active() {
    if (_active >= 0 && _active < _buttons.size())
        _buttons[_active].exec_on_click();
}

} // namespace tgl::scene::state
