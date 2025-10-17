#pragma once

#include <glad/gl.h>

#include "gl/camera.hpp"
#include "gl/window.hpp"
#include "terrain/terrain.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#if true
#include <GLFW/glfw3.h>
#endif

class Scene {
public:
    Scene(tgl::gl::Window win, glm::vec3 cam) :
        _window(std::move(win)), _camera(cam), _terrain(1024, 1024) { }

    void main_loop() {
        setup_win();

        glClearColor(0, 0.55, 1, 1);
        while (!_window.should_close()) {
            update_delta();
            handle_input();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            auto view = _camera.view();
            auto proj = glm::mat4(1);
            proj = glm::perspective(
                glm::radians(80.0f), _window.ratio(), 0.1f, 100.0f
            );

            _terrain.render(view, proj);

            _window.swap_poll();
        }
    }

private:
    tgl::gl::Window _window;
    tgl::gl::Camera _camera;
    tgl::terrain::Terrain _terrain;

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

        // _window.context();
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
