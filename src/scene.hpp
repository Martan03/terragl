#pragma once

#include <glad/gl.h>

#include "gl/camera.hpp"
#include "gl/window.hpp"

#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#if true
#include <GLFW/glfw3.h>
#endif

class Scene {
public:
    Scene(glm::vec3 cam) :
        _camera(cam, glm::vec3(0, 0, -1)), _window(800, 600, "terragl") { }

    void main_loop() {
        setup_win();
        while (!_window.should_close()) {
            update_delta();
            handle_input();

            _window.swap_poll();
        }
    }

private:
    tgl::gl::Window _window;
    tgl::gl::Camera _camera;

    float _last_x = -1;
    float _last_y = -1;

    float _delta = 0;
    float _last = 0;

    struct WindowContext {
        tgl::gl::Window *window;
        Scene *scene;
    } _context;

    void setup_win() {
        _context = { &_window, this };
        glfwSetWindowUserPointer(_window.get(), &_context);

        _window.context();
        glfwSetFramebufferSizeCallback(_window.get(), handle_resize);
        glfwSetInputMode(_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(_window.get(), handle_mouse);
    }

    void update_delta() {
        float current = static_cast<float>(glfwGetTime());
        _delta = current - _last;
        _last = current;
    }

    void handle_input() {
        auto win = _window.get();
        if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(win, true);
        }
        if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
            _camera.process_key(tgl::gl::FORWARD, _delta);
        if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
            _camera.process_key(tgl::gl::BACKWARD, _delta);
        if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
            _camera.process_key(tgl::gl::LEFT, _delta);
        if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
            _camera.process_key(tgl::gl::RIGHT, _delta);
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

    static inline WindowContext *get_context(GLFWwindow *win) {
        return static_cast<WindowContext *>(glfwGetWindowUserPointer(win));
    }
};
