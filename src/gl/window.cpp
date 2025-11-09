#include "window.hpp"

#include <stdexcept>

namespace tgl::gl {

Window::Window(int width, int height, const char *title) :
    _width(width), _height(height) {
    glfwInit(4, 6);

    _window = std::unique_ptr<GLFWwindow, del::Window>(
        glfwCreateWindow(width, height, title, nullptr, nullptr)
    );
    if (_window == nullptr) {
        throw std::runtime_error("Failed to create a window.");
    }
    context();
}

void Window::on_resize(int width, int height) {
    glViewport(0, 0, width, height);
    _width = width;
    _height = height;
}

void Window::swap_poll() const {
    glfwSwapBuffers(get());
    glfwPollEvents();
}

void Window::glfwInit(int major, int minor) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

} // namespace tgl::gl
