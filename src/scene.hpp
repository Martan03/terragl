#pragma once

#include <print>
#include <unordered_set>

#include <glad/gl.h>

#include "gl/camera.hpp"
#include "gl/window.hpp"
#include "terrain/terrain.hpp"
#include "text/text.hpp"
#include "text/text_renderer.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#if true
#include <GLFW/glfw3.h>
#endif

class Scene {
public:
    Scene(tgl::gl::Window win, glm::vec3 cam) :
        _window(std::move(win)), _camera(cam), _terrain(1024, 1024) {
        _singleton = this;
        check_controllers();
    }

    void main_loop() {
        setup_win();

        auto text_renderer = tgl::text::TextRenderer();
        auto text = tgl::text::Text("Test of text", 10, 10);
        text.compile(text_renderer.font());
        
        glClearColor(0, 0.55, 1, 1);
        while (!_window.should_close()) {
            update_delta();
            handle_input();
            handle_controllers();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // auto view = _camera.view();
            // auto proj = glm::mat4(1);
            // proj = glm::perspective(
            //     glm::radians(80.0f), _window.ratio(), 0.1f, 500.0f
            // );

            // _terrain.render(view, proj);
            text_renderer.render(_window, text);

            _window.swap_poll();
        }
    }

private:
    static inline Scene *_singleton = nullptr;

    tgl::gl::Window _window;
    tgl::gl::Camera _camera;
    tgl::terrain::Terrain _terrain;

    std::unordered_set<int> _controllers;
    float _last_x = -1;
    float _last_y = -1;
    float _scroll_y = 0;

    float _delta = 0;
    float _last = 0;

    struct WindowContext {
        tgl::gl::Window *window;
        Scene *scene;
    } _context;

    void setup_win() {
        _context = { &_window, this };
        glfwSetWindowUserPointer(_window.get(), &_context);

        // _window.context();
        glfwSetFramebufferSizeCallback(_window.get(), handle_resize);
        glfwSetInputMode(_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(_window.get(), handle_mouse);
        glfwSetScrollCallback(_window.get(), handle_scroll);
        glfwSetJoystickCallback(joystick_callback);
    }

    void update_delta() {
        float current = static_cast<float>(glfwGetTime());
        _delta = current - _last;
        _last = current;
    }

    void handle_input() {
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
            _camera.process_move(move, _delta);
    }

    void on_mouse(GLFWwindow *window, double xpos_in, double ypos_in) {
        auto xpos = static_cast<float>(xpos_in);
        auto ypos = static_cast<float>(ypos_in);

        if (_last_x == -1 || _last_y == -1) {
            _last_x = xpos;
            _last_y = ypos;
        }

        float xoffset = xpos - _last_x;
        float yoffset = _last_y - ypos;
        _last_x = xpos;
        _last_y = ypos;

        _camera.process_mouse(xoffset, yoffset);
    }

    void handle_controllers() {
        const float deadzone = 0.15f;
        auto get = [&](GLFWgamepadstate &state, int key) {
            auto val = state.axes[key];
            return fabs(val) < deadzone ? 0 : val;
        };

        for (int jid : _controllers) {
            GLFWgamepadstate state;
            if (!glfwGetGamepadState(jid, &state)) {
                continue;
            }

            auto move_x = get(state, GLFW_GAMEPAD_AXIS_LEFT_X);
            auto move_y = -get(state, GLFW_GAMEPAD_AXIS_LEFT_Y);
            if (move_x != 0 || move_y != 0)
                _camera.process_move(glm::vec2(move_x, move_y), _delta);

            auto look_x = get(state, GLFW_GAMEPAD_AXIS_RIGHT_X);
            auto look_y = -get(state, GLFW_GAMEPAD_AXIS_RIGHT_Y);
            if (look_x != 0 || look_y != 0)
                _camera.process_controller(look_x, look_y);

            auto speed = get(state, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) + 1;
            speed -= get(state, GLFW_GAMEPAD_AXIS_LEFT_TRIGGER) + 1;
            if (speed != 0)
                _camera.process_scroll(speed, _delta);
        }
    }

    void check_controllers() {
        for (int jid = 0; jid <= 15; ++jid) {
            if (glfwJoystickIsGamepad(jid)) {
                _controllers.insert(jid);
            }
        }
    }

    static void handle_resize(GLFWwindow *win, int width, int height) {
        auto *ctx = Scene::get_context(win);
        if (!ctx || !ctx->window)
            return;
        ctx->window->on_resize(width, height);
    }

    static void handle_mouse(GLFWwindow *win, double xpos_in, double ypos_in) {
        auto *ctx = Scene::get_context(win);
        if (!ctx || !ctx->scene)
            return;
        ctx->scene->on_mouse(win, xpos_in, ypos_in);
    }

    static void handle_scroll(GLFWwindow *win, double xoff, double yoff) {
        auto *ctx = Scene::get_context(win);
        if (!ctx || !ctx->scene)
            return;
        ctx->scene->_camera.process_scroll(static_cast<float>(yoff));
    }

    static void joystick_callback(int jid, int event) {
        if (!_singleton)
            return;

        if (event == GLFW_CONNECTED && glfwJoystickIsGamepad(jid)) {
            std::println("Connected {}", jid);
            _singleton->_controllers.insert(jid);
        } else if (event == GLFW_DISCONNECTED) {
            _singleton->_controllers.erase(jid);
        }
    }

    static inline WindowContext *get_context(GLFWwindow *win) {
        return static_cast<WindowContext *>(glfwGetWindowUserPointer(win));
    }
};
