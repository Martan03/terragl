#pragma once

#include <memory>
#include <stdexcept>

#include <glad/gl.h>

#if true
#include <GLFW/glfw3.h>
#endif

namespace tgl::gl {

namespace del {
struct Window {
    void operator()(GLFWwindow *win) { glfwDestroyWindow(win); }
};
} // namespace del

class Window {
public:
    Window(int width, int height, const char *title) :
        _width(width), _height(height) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        _window = std::unique_ptr<GLFWwindow, del::Window>(
            glfwCreateWindow(width, height, title, nullptr, nullptr)
        );
        if (_window == nullptr) {
            throw std::runtime_error("Failed to create a window.");
        }
        context();
    }

    GLFWwindow *get() const { return _window.get(); }
    int width() const { return _width; }
    int height() const { return _height; }
    float ratio() const { return (float)_width / (float)_height; }

    void context() const { glfwMakeContextCurrent(get()); }
    int should_close() const { return glfwWindowShouldClose(get()); }

    void on_resize(int width, int height) {
        glViewport(0, 0, width, height);
        _width = width;
        _height = height;
    }

    void swap_poll() const {
        glfwSwapBuffers(get());
        glfwPollEvents();
    }

private:
    std::unique_ptr<GLFWwindow, del::Window> _window;

    int _width, _height;
};

} // namespace tgl::gl
