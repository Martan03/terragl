#include <print>
#include <string_view>

#include <glad/gl.h>

#include "gl/window.hpp"
#include "scene.hpp"

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

int main() {
    glfwInit();
    {
        auto window = tgl::gl::Window(800, 600, "terragl");

        if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
            std::println("Failed to initialize GLAD");
            glfwTerminate();
            return -1;
        }

        glEnable(GL_DEPTH_TEST);
        // glEnable(GL_CULL_FACE);
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(err_callback, nullptr);

        auto scene = Scene(std::move(window), glm::vec3(0, 25, 0));
        scene.main_loop();
    }

    glfwTerminate();
    return 0;
}
