#include "sky.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace tgl::sky {

static constexpr char VERT_SHADER[]{
#embed "shader.vert" suffix(, )
    0
};

static constexpr char FRAG_SHADER[]{
#embed "shader.frag" suffix(, )
    0
};

Sky::Sky() :
    _program(VERT_SHADER, FRAG_SHADER),
    _vbo(GL_ARRAY_BUFFER),
    _ebo(GL_ELEMENT_ARRAY_BUFFER) {
    init();
}

void Sky::render(glm::mat4 view, glm::mat4 proj) {
    _program.use();
    _vao.bind();

    view = glm::mat4(glm::mat3(view));
    auto view_loc = _program.uniform_loc("view");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));

    auto proj_loc = _program.uniform_loc("proj");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj));

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glCullFace(GL_BACK);
    glDepthFunc(GL_LESS);
}

void Sky::init() {
    _vao.bind();
    _vbo.bind();
    _ebo.bind();
    // clang-format off
    float vertices[] = {
        -1.0f, -1.0f,  1.0f, // FBL
         1.0f, -1.0f,  1.0f, // FBR
         1.0f,  1.0f,  1.0f, // FTR
        -1.0f,  1.0f,  1.0f, // FTL
        -1.0f, -1.0f, -1.0f, // BBL
         1.0f, -1.0f, -1.0f, // BBR
         1.0f,  1.0f, -1.0f, // BTR
        -1.0f,  1.0f, -1.0f  // BTL
    };
    unsigned indices[] = {
        0, 1, 2, 2, 3, 0, // front
        1, 5, 6, 6, 2, 1, // right
        7, 6, 5, 5, 4, 7, // back
        4, 0, 3, 3, 7, 4, // left
        4, 5, 1, 1, 0, 4, // bottom
        3, 2, 6, 6, 7, 3  // top
    };
    // clang-format on
    _vbo.set(vertices);
    _ebo.set(indices);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
}

} // namespace tgl::sky
