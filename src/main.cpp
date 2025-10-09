#include <print>
#include <string_view>

#include <glad/gl.h>
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

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

float vertices[] = { -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f };

const char *VER_SHADER = R".(
#version 460 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos, 1.0);
}
).";

const char *FRAG_SHADER = R".(
#version 460 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
).";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "terragl", NULL, NULL);
    if (window == NULL) {
        std::println("Failed to create a window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::println("Failed to initialize GLAD");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(err_callback, nullptr);

    auto ver_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(ver_shader, 1, &VER_SHADER, NULL);
    glCompileShader(ver_shader);

    int success;
    glGetShaderiv(ver_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(ver_shader, 512, NULL, infoLog);
        std::println("Error: {}", infoLog);
        return -1;
    }

    auto frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &FRAG_SHADER, NULL);
    glCompileShader(frag_shader);

    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(frag_shader, 512, NULL, infoLog);
        std::println("Error: {}", infoLog);
        return -1;
    }

    // TODO: check valid creation
    auto shader_program = glCreateProgram();
    glAttachShader(shader_program, ver_shader);
    glAttachShader(shader_program, frag_shader);
    // TODO: check error (glGetProgramiv and glGetProgramInfoLog)
    glLinkProgram(shader_program);

    glDeleteShader(ver_shader);
    glDeleteShader(frag_shader);

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0
    );
    glEnableVertexAttribArray(0);

    glClearColor(0.2f, 0.8f, 0.7f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
