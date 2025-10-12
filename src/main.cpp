#include <print>
#include <string_view>

#include <glad/gl.h>

#include "gl/buffer.hpp"
#include "gl/program.hpp"
#include "gl/texture.hpp"
#include "gl/vertex_array.hpp"
#include "gl/window.hpp"
#include "height_map/height_map.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
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
    // positions   // texcoords
    -0.5f, -0.5f, -0.5, 0.0f, 0.0f, // front bottom left 0
    0.5f,  -0.5f, -0.5, 1.0f, 0.0f, // front bottom right 1
    -0.5f, 0.5f,  -0.5, 0.0f, 1.0f, // front top left 2
    0.5f,  0.5f,  -0.5, 1.0f, 1.0f, // front top right 3
    -0.5f, -0.5f, 0.5,  1.0f, 1.0f, // back bottom right 4
    0.5f,  -0.5f, 0.5,  0.0f, 1.0f, // back bottom left 5
    -0.5f, 0.5f,  0.5,  1.0f, 0.0f, // back top right 6
    0.5f,  0.5f,  0.5,  0.0f, 0.0f, // back top left 7
};

unsigned int indices[] = { 0, 1, 2, 1, 2, 3, 1, 5, 3, 5, 3, 7,
                           5, 4, 7, 4, 7, 6, 4, 0, 6, 0, 6, 2,
                           2, 3, 6, 3, 6, 7, 0, 1, 4, 1, 4, 5 };

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(-1.7f, 3.0f, -7.5f),  glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.6f, 2.0f, -2.9f),   glm::vec3(2.0f, 0.2f, -3.0f),
    glm::vec3(-1.7f, 1.0f, -2.0f)
};

const char *VER_SHADER = R".(
#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 TexCoord;

void main() {
    gl_Position = proj * view * model * vec4(pos, 1.0);
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(err_callback, nullptr);

    auto program = tgl::gl::Program(VER_SHADER, FRAG_SHADER);

    auto vao = tgl::gl::VertexArray();
    vao.bind();

    auto vbo = tgl::gl::Buffer(GL_ARRAY_BUFFER);
    vbo.bind();
    vbo.set(vertices);

    auto ebo = tgl::gl::Buffer(GL_ELEMENT_ARRAY_BUFFER);
    ebo.bind();
    ebo.set(indices);

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void *)(3 * sizeof(float))
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

    auto view_mat = glm::mat4(1.0f);
    view_mat = glm::translate(view_mat, glm::vec3(0.0f, 0.0f, -4));

    while (!window.should_close()) {
        window.handle_input();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto proj_mat = glm::mat4(1);
        proj_mat = glm::perspective(
            glm::radians(45.0f), window.ratio(), 0.1f, 100.0f
        );

        auto view_loc = program.uniform_loc("view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_mat));
        auto proj_loc = program.uniform_loc("proj");
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj_mat));

        for (int i = 0; i < 9; ++i) {
            auto model_mat = glm::mat4(1);
            model_mat = glm::translate(model_mat, cubePositions[i]);
            float ang = 20.0f * i;
            model_mat = glm::rotate(
                model_mat,
                (float)glfwGetTime() * glm::radians(50.0f) + ang,
                glm::vec3(0.5, 1, 0)
            );

            auto model_loc = program.uniform_loc("model");
            glUniformMatrix4fv(
                model_loc, 1, GL_FALSE, glm::value_ptr(model_mat)
            );
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        window.swap_poll();
    }

    glfwTerminate();
    return 0;
}
