#include "scene.hpp"

#include "state/game.hpp"
#include "state/menu.hpp"

namespace tgl::scene {

Scene::Scene(gl::Window win, glm::vec3 cam) :
    _window(std::move(win)), _camera(cam), _terrain(1024, 1024) {
    _singleton = this;
    check_controllers();

    _states.emplace(StateType::Game, std::make_unique<state::Game>(*this));
    _states.emplace(StateType::Menu, std::make_unique<state::Menu>(*this));
    set_state(StateType::Game);
}

void Scene::main_loop() {
    setup_win();

    glClearColor(0, 0.55, 1, 1);
    while (!_window.should_close()) {
        update_delta();
        _active->handle_input(_delta);
        _active->handle_controllers(_delta);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _active->render();

        _window.swap_poll();
    }
}

void Scene::set_state(StateType type) {
    switch (type) {
    case tgl::scene::StateType::Game:
        glfwSetInputMode(_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        break;
    case tgl::scene::StateType::Menu:
        glfwSetInputMode(_window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        break;
    }
    _active = _states.at(type).get();
}

void Scene::setup_win() {
    glfwSetWindowUserPointer(_window.get(), this);

    // _window.context();
    glfwSetFramebufferSizeCallback(_window.get(), handle_resize);
    glfwSetCursorPosCallback(_window.get(), handle_mouse);
    glfwSetScrollCallback(_window.get(), handle_scroll);
    glfwSetJoystickCallback(joystick_callback);
}

void Scene::update_delta() {
    float current = static_cast<float>(glfwGetTime());
    _delta = current - _last;
    _last = current;
}

void Scene::handle_resize(GLFWwindow *win, int width, int height) {
    auto *ctx = Scene::get_context(win);
    if (!ctx)
        return;
    ctx->window().on_resize(width, height);
}

void Scene::handle_mouse(GLFWwindow *win, double xpos_in, double ypos_in) {
    auto *ctx = Scene::get_context(win);
    if (!ctx)
        return;
    ctx->_active->handle_mouse(xpos_in, ypos_in);
}

void Scene::handle_scroll(GLFWwindow *win, double xoff, double yoff) {
    auto *ctx = Scene::get_context(win);
    if (!ctx)
        return;
    ctx->_active->handle_scroll(xoff, yoff);
}

void Scene::check_controllers() {
    for (int jid = 0; jid <= 15; ++jid) {
        if (glfwJoystickIsGamepad(jid)) {
            _controllers.insert(jid);
        }
    }
}

void Scene::joystick_callback(int jid, int event) {
    if (!_singleton)
        return;

    if (event == GLFW_CONNECTED && glfwJoystickIsGamepad(jid)) {
        _singleton->_controllers.insert(jid);
    } else if (event == GLFW_DISCONNECTED) {
        _singleton->_controllers.erase(jid);
    }
}

inline Scene *Scene::get_context(GLFWwindow *win) {
    return static_cast<Scene *>(glfwGetWindowUserPointer(win));
}

} // namespace tgl::scene
