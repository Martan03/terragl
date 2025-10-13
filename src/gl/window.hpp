#pragma once

#include <stdexcept>

#include <glad/gl.h>

#if true
#include <GLFW/glfw3.h>
#endif

namespace tgl::gl {

class Window {
public:
    Window(int width, int height, const char *title) :
        _width(width), _height(height) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        _window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (_window == nullptr) {
            throw std::runtime_error("Failed to create a window.");
        }

        context();
        glfwSetWindowUserPointer(_window, this);
        glfwSetFramebufferSizeCallback(_window, fb_size_callback);
    }

    ~Window() {
        if (_window != nullptr) {
            glfwDestroyWindow(_window);
            _window = nullptr;
        }
    }

    GLFWwindow *get() { return _window; }
    int width() { return _width; }
    int height() { return _height; }
    float ratio() { return (float)_width / (float)_height; }

    void context() { glfwMakeContextCurrent(_window); }
    int should_close() { return glfwWindowShouldClose(_window); }

    void swap_poll() {
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

private:
    GLFWwindow *_window = nullptr;

    int _width, _height;

    void on_resize(int width, int height) {
        glViewport(0, 0, width, height);
        _width = width;
        _height = height;
    }

    static void fb_size_callback(GLFWwindow *win, int width, int height) {
        auto *self = static_cast<Window *>(glfwGetWindowUserPointer(win));
        if (self) {
            self->on_resize(width, height);
        }
    }
};

} // namespace tgl::gl
