#include "terrain.hpp"

#include "../gl/program.hpp"
#include "../height_map/height_map.hpp"
#include "glad/gl.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace tgl::terrain {

static constexpr char VERT_SHADER[]{
#embed "shader.vert" suffix(, )
    0
};

static constexpr char FRAG_SHADER[]{
#embed "shader.frag" suffix(, )
    0
};

Terrain::Terrain(int width, int height) :
    _program(VERT_SHADER, FRAG_SHADER),
    _vbo(GL_ARRAY_BUFFER),
    _ebo(GL_ELEMENT_ARRAY_BUFFER) {
    init_buffers(width, height);
    vertex_attrib();
    set_static_uniforms();
}

void Terrain::render(glm::mat4 view, glm::mat4 proj) {
    _program.use();
    _vao.bind();

    auto model_mat = glm::mat4(1);
    model_mat = glm::translate(model_mat, glm::vec3(-128, 0, -128));
    auto model_loc = _program.uniform_loc("model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_mat));

    auto view_loc = _program.uniform_loc("view");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));

    auto proj_loc = _program.uniform_loc("proj");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj));

    glDrawElements(GL_TRIANGLES, _triangle_cnt, GL_UNSIGNED_INT, 0);
}

void Terrain::init_buffers(int width, int height) {
    auto heights = height_map::HeightMap(width, height);
    heights.perlin_gen(5);
    heights.hydro_erosion();
    _vao.bind();

    _vbo.bind();
    _vbo.set(heights.vertices());

    _ebo.bind();
    auto indices = heights.indices();
    _triangle_cnt = indices.size();
    _ebo.set(indices);
}

void Terrain::vertex_attrib() {
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, sizeof(height_map::Vertex), (void *)0
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
}

void Terrain::set_static_uniforms() {
    _program.use();
    auto lpos_loc = _program.uniform_loc("lightPos");
    glUniform3f(lpos_loc, 128, 128, 0);
    auto light_loc = _program.uniform_loc("lightColor");
    glUniform3f(light_loc, 1, 1, 1);
}

} // namespace tgl::terrain
