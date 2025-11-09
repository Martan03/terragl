#pragma once

#include <memory>

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
    Window(int width, int height, const char *title);

    GLFWwindow *get() const { return _window.get(); }
    int width() const { return _width; }
    int height() const { return _height; }
    float ratio() const { return (float)_width / (float)_height; }

    void context() const { glfwMakeContextCurrent(get()); }
    int should_close() const { return glfwWindowShouldClose(get()); }

    void on_resize(int width, int height);
    void swap_poll() const;

private:
    std::unique_ptr<GLFWwindow, del::Window> _window;

    int _width, _height;

    void glfwInit(int major, int minor);
};

} // namespace tgl::gl
