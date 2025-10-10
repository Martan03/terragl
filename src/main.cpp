#include <print>
#include <string_view>

#include <glad/gl.h>

#include "gl/buffer.hpp"
#include "gl/program.hpp"
#include "gl/vertex_array.hpp"
#include "gl/window.hpp"

#include <glm/glm.hpp>
#if true
#include <GLFW/glfw3.h>
#endif

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void err_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam
) {
    std::println("{}", std::string_view(message, length));
}

float vertices[] = {
    0.5f,  0.5f,  0.0f, 1, 0, 0, // top right
    0.5f,  -0.5f, 0.0f, 0, 1, 0, // bottom right
    -0.5f, -0.5f, 0.0f, 1, 0, 0, // bottom left
    -0.5f, 0.5f,  0.0f, 0, 0, 1, // top left
};
unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

const char *VER_SHADER = R".(
#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

out vec3 color;

void main() {
    gl_Position = vec4(pos, 1.0);
    color = col;
}
).";

const char *FRAG_SHADER = R".(
#version 460 core
in vec3 color;

out vec4 FragColor;

void main() {
    FragColor = vec4(color, 1);
}
).";

int main() {
    glfwInit();

    auto window = tgl::gl::Window(800, 600, "terragl");
    window.context();
    glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::println("Failed to initialize GLAD");
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(err_callback, nullptr);

    auto program = tgl::gl::Program(VER_SHADER, FRAG_SHADER);

    auto vao = tgl::gl::VertexArray();
    vao.bind();

    auto vbo = tgl::gl::Buffer();
    vbo.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    auto ebo = tgl::gl::Buffer();
    ebo.bind(GL_ELEMENT_ARRAY_BUFFER);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void *)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    program.use();
    glClearColor(0.0f, 0.1f, 0.1f, 1.0f);

    while (!window.should_close()) {
        window.handle_input();
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.swap_poll();
    }

    glfwTerminate();
    return 0;
}
