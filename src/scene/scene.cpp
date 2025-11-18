#include "scene.hpp"

#include <format>

#include <GLFW/glfw3.h>

#include "state/game.hpp"
#include "state/menu.hpp"

namespace tgl::scene {

Scene::Scene(gl::Window win, glm::vec3 cam) :
    _window(std::move(win)),
    _camera(cam),
    _terrain(1024, 1024),
    _text_sys(),
    _fps(glm::vec2(10, 20), glm::vec2(-1, 1), _text_sys, "0 FPS") {
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
        _fps.render();

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
    _active->resize();
}

void Scene::setup_win() {
    glfwSetWindowUserPointer(_window.get(), this);

    // _window.context();
    glfwSetFramebufferSizeCallback(_window.get(), handle_resize);
    glfwSetCursorPosCallback(_window.get(), handle_mouse);
    glfwSetMouseButtonCallback(_window.get(), handle_click);
    glfwSetScrollCallback(_window.get(), handle_scroll);
    glfwSetKeyCallback(_window.get(), handle_key);
    glfwSetJoystickCallback(joystick_callback);
}

void Scene::update_delta() {
    float current = static_cast<float>(glfwGetTime());
    _delta = current - _last;
    _last = current;

    ++_frames;
    if (current - _last_fps >= 1.0) {
        int fps = _frames / (current - _last_fps);
        _fps.set_text(std::format("{} FPS", fps));
        _last_fps = current;
        _frames = 0;
    }
}

void Scene::handle_resize(GLFWwindow *win, int width, int height) {
    auto *ctx = Scene::get_context(win);
    if (!ctx)
        return;
    ctx->window().on_resize(width, height);
    ctx->_active->resize();

    auto proj = ctx->window().ortho();
    ctx->_text_sys.set_proj(proj);
}

void Scene::handle_mouse(GLFWwindow *win, double xpos_in, double ypos_in) {
    auto *ctx = Scene::get_context(win);
    if (!ctx)
        return;

    if (ctx->_mouse_x == -1 || ctx->_mouse_y == -1) {
        ctx->_mouse_x = xpos_in;
        ctx->_mouse_y = ypos_in;
    }

    ctx->_active->handle_mouse(xpos_in, ypos_in);
    ctx->_mouse_x = xpos_in;
    ctx->_mouse_y = ypos_in;
}

void Scene::handle_click(GLFWwindow *win, int button, int action, int mods) {
    auto *ctx = Scene::get_context(win);
    if (!ctx)
        return;
    ctx->_active->handle_click(button, action, mods);
}

void Scene::handle_scroll(GLFWwindow *win, double xoff, double yoff) {
    auto *ctx = Scene::get_context(win);
    if (!ctx)
        return;
    ctx->_active->handle_scroll(xoff, yoff);
}

void Scene::handle_key(
    GLFWwindow *win, int key, int scancode, int action, int mods
) {
    auto *ctx = Scene::get_context(win);
    if (!ctx)
        return;
    ctx->_active->handle_key(key, scancode, action, mods);
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
