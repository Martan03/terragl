#include <print>
#include <string_view>

#include <glad/gl.h>

#include "gl/buffer.hpp"
#include "gl/program.hpp"
#include "gl/texture.hpp"
#include "gl/vertex_array.hpp"
#include "gl/window.hpp"
#include "height_map/height_map.hpp"

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

float quadVertices[] = {
    // positions   // texcoords
    -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f
};

const char *VER_SHADER = R".(
#version 460 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;

out vec2 TexCoord;

void main() {
    gl_Position = vec4(pos, 0.0, 1.0);
    TexCoord = tex;
}
).";

const char *FRAG_SHADER = R".(
#version 460 core
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D heightMap;

void main() {
    float h = texture(heightMap, TexCoord).r;
    FragColor = vec4(h, h, h, 1);
}
).";

int main() {
    glfwInit();

    auto window = tgl::gl::Window(800, 600, "terragl");

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

    auto vbo = tgl::gl::Buffer(GL_ARRAY_BUFFER);
    vbo.bind();
    vbo.set(quadVertices);

    // auto ebo = tgl::gl::Buffer(GL_ELEMENT_ARRAY_BUFFER);
    // ebo.bind();
    // ebo.set(indices);

    glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void *)(2 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    auto heights = tgl::height_map::HeightMap(256, 256);
    heights.perlin_gen(5);
    auto pixels = heights.pixels();

    auto texture = tgl::gl::Texture();
    texture.bind();
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        256,
        256,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        pixels.data()
    );
    texture.param(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture.param(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture.param(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture.param(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    program.use();
    glClearColor(0.0f, 0.1f, 0.1f, 1.0f);

    while (!window.should_close()) {
        window.handle_input();
        glClear(GL_COLOR_BUFFER_BIT);

        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        window.swap_poll();
    }

    glfwTerminate();
    return 0;
}
