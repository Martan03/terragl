#include "water.hpp"

namespace tgl::water {

static constexpr char VERT_SHADER[]{
#embed "../terrain/shader.vert" suffix(, )
    0
};

static constexpr char TESC_SHADER[]{
#embed "../terrain/shader.tesc" suffix(, )
    0
};

static constexpr char TESE_SHADER[]{
#embed "shader.tese" suffix(, )
    0
};

static constexpr char FRAG_SHADER[]{
#embed "shader.frag" suffix(, )
    0
};

Water::Water(gl::Camera &camera, terrain::Terrain &terrain) :
    _camera(camera),
    _terrain(terrain),
    _program(VERT_SHADER, TESC_SHADER, TESE_SHADER, FRAG_SHADER),
    _tex(GL_TEXTURE1) {
    init();
}

void Water::render(
    glm::mat4 view, glm::mat4 proj, glm::vec3 sunPos, float time
) {
    _program.use();
    _terrain.height_texture().bind(GL_TEXTURE0);
    _terrain.depth_texture().bind(GL_TEXTURE2);
    _tex.bind();

    _program.uniform("view", view);
    _program.uniform("lightMat", _terrain.light_matrix());
    _program.uniform("proj", proj);
    _program.uniform("sunPos", sunPos);
    _program.uniform("viewPos", _camera.position());

    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    _terrain.vao().bind();
    glDrawElements(GL_PATCHES, _terrain.triangle_cnt(), GL_UNSIGNED_INT, 0);
    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
}

void Water::update(float delta) {
    offset += delta;
    _program.use();
    _program.uniform("time", offset);
}

void Water::init() {
    _program.use();

    auto model_mat = glm::translate(glm::mat4(1), glm::vec3(-128, 0, -128));
    _program.uniform("model", model_mat);

    _program.uniform("heightTex", 0);
    _program.uniform("waterTex", 1);
    _program.uniform("depthTex", 2);
    auto lightCol = glm::vec3(1, 1, 1);
    _program.uniform("lightColor", lightCol);
    _program.uniform("amp", _terrain.map().amp());
    gen_tex();
    _tex.wrap(GL_CLAMP_TO_EDGE);
    _tex.filter(GL_LINEAR);
}

void Water::gen_tex() {
    _tex.bind();
    auto data = _terrain.map().water();
    int w = _terrain.map().width(), h = _terrain.map().height();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        _tex.kind(), 0, GL_R32F, w, h, 0, GL_RED, GL_FLOAT, data.data()
    );
}

} // namespace tgl::water
