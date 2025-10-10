#pragma once

#include <stdexcept>

#include <glad/gl.h>

#if true
#include <GLFW/glfw3.h>
#endif

namespace tgl::gl {

class Window {
public:
    Window(int width, int height, const char *title) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        this->window =
            glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (window == nullptr) {
            throw std::runtime_error("Failed to create a window.");
        }
    }

    ~Window() {
        if (window != nullptr) {
            glfwDestroyWindow(this->window);
            this->window = nullptr;
        }
    }

    GLFWwindow *get() { return this->window; }

    void context() { glfwMakeContextCurrent(this->window); }
    int should_close() { return glfwWindowShouldClose(this->window); }

    void swap_poll() {
        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }

    void handle_input() {
        if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(this->window, true);
        }
    }

private:
    GLFWwindow *window = nullptr;
};

} // namespace tgl::gl
