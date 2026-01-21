#include "settings.hpp"

#include <algorithm>
#include <memory>

#include "../../gui/button.hpp"
#include "../../gui/text/text.hpp"
#include "../scene.hpp"

namespace tgl::scene::state {

Settings::Settings(Scene &scene) : State(scene) {
    auto &tsys = scene.text_sys();
    auto ft = gui::Text(glm::vec2(5, 60), tsys, "FBM type:");
    _widgets.push_back(std::make_unique<gui::Text>(std::move(ft)));

    auto pad = glm::vec2(10, 15);
    auto normal_fbm =
        gui::Button(glm::vec2(10, 70), glm::vec2(125, 35), tsys, "Normal");
    normal_fbm.padding(pad);
    normal_fbm.bg(glm::vec3(0, 0, 0));
    normal_fbm.set_on_click([this]() {
        _scene.game_state()->terrain().set_fbm(height_map::FbmType::Normal);
        _scene.game_state()->water().regenerate();
    });
    _widgets.push_back(std::make_unique<gui::Button>(std::move(normal_fbm)));

    auto ridged_fbm =
        gui::Button(glm::vec2(10, 110), glm::vec2(125, 35), tsys, "Ridged");
    ridged_fbm.padding(pad);
    ridged_fbm.bg(glm::vec3(0, 0, 0));
    ridged_fbm.set_on_click([this]() {
        _scene.game_state()->terrain().set_fbm(height_map::FbmType::Ridged);
        _scene.game_state()->water().regenerate();
    });
    _widgets.push_back(std::make_unique<gui::Button>(std::move(ridged_fbm)));

    layout();
}

void Settings::render() {
    _scene.game_state()->terrain().update();
    _scene.game_state()->render();
    for (auto &widget : _widgets) {
        widget->render();
    }
}

void Settings::resize() {
    auto proj = _scene.window().ortho();
    for (auto &widget : _widgets) {
        widget->set_proj(proj);
    }
    layout();
}

void Settings::handle_key(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        _scene.set_state(StateType::Game);
    }
}

void Settings::handle_click(int button, int action, int mods) {
    for (auto &widget : _widgets) {
        widget->on_mouse_click(
            button, action, _scene.mouse_x(), _scene.mouse_y()
        );
    }
}

void Settings::handle_controller(GLFWgamepadstate &state, int jid) {
    auto isPressed = [&](int key) {
        return state.buttons[key] && !_scene.contoller_pressed(jid, key);
    };

    if (isPressed(GLFW_GAMEPAD_BUTTON_START) ||
        isPressed(GLFW_GAMEPAD_BUTTON_B)) {
        _scene.set_state(StateType::Game);
    }
}

void Settings::layout() {
    auto w = _scene.window().width(), h = _scene.window().height();

    auto mw = w - 155, mh = h - 20;
    auto ms = std::max(std::min(mw, mh), 0);
    auto mx = float(mw - ms) / 2.0f + 145;
    auto my = float(mh - ms) / 2.0f + 10;
}

} // namespace tgl::scene::state
