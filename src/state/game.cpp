#include "game.hpp"

namespace tgl::state {

Game::Game(Scene &scene, gl::Window &window) :
    State(scene, window),
    _camera(glm::vec3(0, 25, 0)),
    _terrain(1024, 1024) { }

void Game::handle_input(float delta) {
    auto win = _window.get();
    auto move = glm::vec2(0, 0);
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(win, true);

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

void Game::handle_mouse(double xpos, double ypos) {
    auto x = static_cast<float>(xpos);
    auto y = static_cast<float>(ypos);

    if (_last_x == -1 || _last_y == -1) {
        _last_x = xpos;
        _last_y = ypos;
    }

    _camera.process_mouse(xpos - _last_x, _last_y - ypos);
    _last_x = xpos;
    _last_y = ypos;
}

void Game::render() {
    auto view = _camera.view();
    auto proj = glm::mat4(1);
    proj =
        glm::perspective(glm::radians(80.0f), _window.ratio(), 0.1f, 500.0f);

    _terrain.render(view, proj);
}

} // namespace tgl::state
