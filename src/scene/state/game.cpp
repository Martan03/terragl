#include "game.hpp"

#include "../scene.hpp"

#if true
#include <GLFW/glfw3.h>
#endif

namespace tgl::scene::state {

Game::Game(Scene &scene) :
    State(scene),
    _camera(glm::vec3(0, 25, 0)),
    _terrain(scene.window(), 1024, 1024),
    _water(_terrain),
    _map(glm::vec2(0, 0), glm::vec2(250, 250), &_water.texture()) { }

void Game::render() {
    auto view = _camera.view();
    auto proj = glm::mat4(1);
    auto ratio = _scene.window().ratio();
    proj = glm::perspective(glm::radians(80.0f), ratio, 0.1f, 500.0f);

    _terrain.update();
    _sky.update(_scene.delta());
    glEnable(GL_DEPTH_TEST);
    _terrain.render(view, proj, _sky.sunPos);
    _sky.render(view, proj);
    _water.render(view, proj, _sky.sunPos);
    glDisable(GL_DEPTH_TEST);

    _map.render();
}

void Game::resize() {
    auto proj = _scene.window().ortho();
    _map.set_proj(proj);
    _map.set_pos(glm::vec2(_scene.window().width() - 255, 5));
}

void Game::handle_input(float delta) {
    auto win = _scene.window().get();
    auto move = glm::vec2(0, 0);

    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
        move.y += 1;
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
        move.y -= 1;
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
        move.x -= 1;
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        move.x += 1;

    if (move.y != 0 || move.x != 0)
        _camera.process_move(move, delta);
}

void Game::handle_key(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        _scene.set_state(StateType::Menu);

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        _sky.toggleDayCycle();
}

void Game::handle_mouse(float xpos, float ypos) {
    _camera.process_mouse(xpos - _scene.mouse_x(), _scene.mouse_y() - ypos);
}

void Game::handle_scroll(float xoff, float yoff) {
    _camera.process_scroll(yoff);
}

void Game::handle_controllers(float delta) {
    const float deadzone = 0.15f;
    auto get = [&](GLFWgamepadstate &state, int key) {
        auto val = state.axes[key];
        return fabs(val) < deadzone ? 0 : val;
    };

    for (int jid : _scene.controllers()) {
        GLFWgamepadstate state;
        if (!glfwGetGamepadState(jid, &state)) {
            continue;
        }

        auto move_x = get(state, GLFW_GAMEPAD_AXIS_LEFT_X);
        auto move_y = -get(state, GLFW_GAMEPAD_AXIS_LEFT_Y);
        if (move_x != 0 || move_y != 0)
            _camera.process_move(glm::vec2(move_x, move_y), delta);

        auto look_x = get(state, GLFW_GAMEPAD_AXIS_RIGHT_X);
        auto look_y = -get(state, GLFW_GAMEPAD_AXIS_RIGHT_Y);
        if (look_x != 0 || look_y != 0)
            _camera.process_controller(look_x, look_y);

        auto speed = get(state, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) + 1;
        speed -= get(state, GLFW_GAMEPAD_AXIS_LEFT_TRIGGER) + 1;
        if (speed != 0)
            _camera.process_scroll(speed, delta);
    }
}

} // namespace tgl::scene::state
