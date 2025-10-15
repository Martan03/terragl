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
    }

    ~Window() {
        if (_window != nullptr) {
            glfwDestroyWindow(_window);
            _window = nullptr;
        }
    }

    GLFWwindow *get() const { return _window; }
    int width() const { return _width; }
    int height() const { return _height; }
    float ratio() const { return (float)_width / (float)_height; }

    void context() const { glfwMakeContextCurrent(_window); }
    int should_close() const { return glfwWindowShouldClose(_window); }

    void on_resize(int width, int height) {
        glViewport(0, 0, width, height);
        _width = width;
        _height = height;
    }

    void swap_poll() const {
        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

private:
    GLFWwindow *_window = nullptr;

    int _width, _height;
};

} // namespace tgl::gl
