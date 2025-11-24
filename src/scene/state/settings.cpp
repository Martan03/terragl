#include "settings.hpp"

#include <memory>

#include "../../gui/button.hpp"
#include "../../gui/text/text.hpp"
#include "../scene.hpp"

namespace tgl::scene::state {

Settings::Settings(Scene &scene) : State(scene) {
    auto &tsys = scene.text_sys();
    auto nt = gui::Text(glm::vec2(5, 60), tsys, "Noise type:");
    _widgets.push_back(std::make_unique<gui::Text>(std::move(nt)));

    auto pad = glm::vec2(10, 15);
    auto perlin =
        gui::Button(glm::vec2(10, 70), glm::vec2(125, 35), tsys, "Perlin");
    perlin.padding(pad);
    _widgets.push_back(std::make_unique<gui::Button>(std::move(perlin)));
    auto simplex =
        gui::Button(glm::vec2(10, 110), glm::vec2(125, 35), tsys, "Simplex");
    simplex.padding(pad);
    _widgets.push_back(std::make_unique<gui::Button>(std::move(simplex)));
}

void Settings::render() {
    for (auto &widget : _widgets) {
        widget->render();
    }
}

void Settings::resize() {
    auto proj = _scene.window().ortho();
    for (auto &widget : _widgets) {
        widget->set_proj(proj);
    }
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

void Settings::layout() {
    auto w = _scene.window().width(), h = _scene.window().height();
}

} // namespace tgl::scene::state
