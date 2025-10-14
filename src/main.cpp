#include <print>
#include <string_view>

#include <glad/gl.h>

#include "gl/buffer.hpp"
#include "gl/camera.hpp"
#include "gl/program.hpp"
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

auto camera = tgl::gl::Camera(glm::vec3(0, 5, 0), glm::vec3(0, 0, -1));
float last_x = -1;
float last_y = -1;

float delta = 0;
float last = 0;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void handle_input(tgl::gl::Window &window, tgl::gl::Camera &camera) {
    auto win = window.get();
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window.get(), true);
    }
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
        camera.process_key(tgl::gl::FORWARD, delta);
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
        camera.process_key(tgl::gl::BACKWARD, delta);
    if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
        camera.process_key(tgl::gl::LEFT, delta);
    if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
        camera.process_key(tgl::gl::RIGHT, delta);
}

void handle_mouse(GLFWwindow *window, double xpos_in, double ypos_in) {
    auto xpos = static_cast<float>(xpos_in);
    auto ypos = static_cast<float>(ypos_in);

    if (last_x == -1 || last_y == -1) {
        last_x = xpos;
        last_y = ypos;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos;
    last_x = xpos;
    last_y = ypos;

    camera.process_mouse(xoffset, yoffset);
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

const char *VER_SHADER = R".(
#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;

out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = proj * view * model * vec4(pos, 1);
    fragPos = vec3(model * vec4(pos, 1));
    normal = norm;
}
).";

const char *FRAG_SHADER = R".(
#version 460 core
in vec3 fragPos;
in vec3 normal;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;

void main() {
    vec3 ambient = 0.1 * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, lightDir), 0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * vec3(0, 0.5, 0);
    FragColor = vec4(result, 1);
}
).";

int main() {
    glfwInit();

    auto window = tgl::gl::Window(800, 600, "terragl");
    glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.get(), handle_mouse);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::println("Failed to initialize GLAD");
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(err_callback, nullptr);

    auto program = tgl::gl::Program(VER_SHADER, FRAG_SHADER);

    auto heights = tgl::height_map::HeightMap(256, 256);
    heights.perlin_gen(5);

    auto vao = tgl::gl::VertexArray();
    vao.bind();

    auto vbo = tgl::gl::Buffer(GL_ARRAY_BUFFER);
    vbo.bind();
    vbo.set(heights.vertices());

    auto ebo = tgl::gl::Buffer(GL_ELEMENT_ARRAY_BUFFER);
    ebo.bind();
    ebo.set(heights.indices());
    auto indices_cnt = heights.indices().size();

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, sizeof(tgl::height_map::Vertex), (void *)0
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(tgl::height_map::Vertex),
        (void *)(sizeof(glm::vec3))
    );
    glEnableVertexAttribArray(1);

    program.use();
    glClearColor(0.0f, 0.1f, 0.1f, 1.0f);

    auto lpos_loc = program.uniform_loc("lightPos");
    glUniform3f(lpos_loc, 128, 128, 0);
    auto light_loc = program.uniform_loc("lightColor");
    glUniform3f(light_loc, 1, 1, 1);

    while (!window.should_close()) {
        float current = static_cast<float>(glfwGetTime());
        delta = current - last;
        last = current;

        handle_input(window, camera);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto model_mat = glm::mat4(1);
        model_mat = glm::translate(model_mat, glm::vec3(-128, 0, -128));
        auto model_loc = program.uniform_loc("model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_mat));

        auto view_mat = camera.view();
        auto view_loc = program.uniform_loc("view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_mat));

        auto proj_mat = glm::mat4(1);
        proj_mat = glm::perspective(
            glm::radians(45.0f), window.ratio(), 0.1f, 100.0f
        );
        auto proj_loc = program.uniform_loc("proj");
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj_mat));

        glDrawElements(GL_TRIANGLES, indices_cnt, GL_UNSIGNED_INT, 0);

        window.swap_poll();
    }

    glfwTerminate();
    return 0;
}
