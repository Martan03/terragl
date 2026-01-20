#include "game.hpp"

#include "../scene.hpp"

namespace tgl::scene::state {

Game::Game(Scene &scene) :
    State(scene),
    _camera(glm::vec3(0, 25, 0)),
    _terrain(scene.window(), 1024, 1024),
    _water(_camera, _terrain) { }

void Game::render() {
    auto view = _camera.view();
    auto proj = glm::mat4(1);
    auto ratio = _scene.window().ratio();
    proj = glm::perspective(glm::radians(80.0f), ratio, 0.1f, 500.0f);

    glEnable(GL_DEPTH_TEST);
    _terrain.render(view, proj, _sky.sunPos);
    _sky.render(view, proj);
    _water.render(view, proj, _sky.sunPos, _scene.time());
    glDisable(GL_DEPTH_TEST);
}

void Game::update() {
    _terrain.update();
    _sky.update(_scene.delta());
    _water.update(_scene.delta());
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

    auto fly = 0.0f;
    if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS)
        fly += 1.0f;
    if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        fly -= 1.0f;
    if (fly != 0)
        _camera.process_fly(fly, delta);
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

void Game::handle_controller(GLFWgamepadstate &state, int jid) {
    auto isPressed = [&](int key) {
        return state.buttons[key] && !_scene.contoller_pressed(jid, key);
    };
    auto delta = _scene.delta();

    auto move_x = getGpAxes(state, GLFW_GAMEPAD_AXIS_LEFT_X);
    auto move_y = -getGpAxes(state, GLFW_GAMEPAD_AXIS_LEFT_Y);
    if (move_x != 0 || move_y != 0)
        _camera.process_move(glm::vec2(move_x, move_y), delta);

    auto fly = 0.0f;
    if (state.buttons[GLFW_GAMEPAD_BUTTON_A])
        fly += 1.0f;
    if (state.buttons[GLFW_GAMEPAD_BUTTON_B])
        fly -= 1.0f;
    _camera.process_fly(fly, delta);

    auto look_x = getGpAxes(state, GLFW_GAMEPAD_AXIS_RIGHT_X);
    auto look_y = -getGpAxes(state, GLFW_GAMEPAD_AXIS_RIGHT_Y);
    if (look_x != 0 || look_y != 0)
        _camera.process_controller(look_x, look_y);

    auto speed = getGpAxes(state, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) + 1;
    speed -= getGpAxes(state, GLFW_GAMEPAD_AXIS_LEFT_TRIGGER) + 1;
    if (speed != 0)
        _camera.process_scroll(speed, delta);

    if (isPressed(GLFW_GAMEPAD_BUTTON_START))
        _scene.set_state(StateType::Menu);
    if (isPressed(GLFW_GAMEPAD_BUTTON_Y))
        _sky.toggleDayCycle();
}

} // namespace tgl::scene::state
