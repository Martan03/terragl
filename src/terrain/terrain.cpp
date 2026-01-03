#include "terrain.hpp"

#include <iostream>

#include "../gl/program.hpp"
#include "glad/gl.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace tgl::terrain {

static constexpr char VERT_SHADER[]{
#embed "shader.vert" suffix(, )
    0
};

static constexpr char TESC_SHADER[]{
#embed "shader.tesc" suffix(, )
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

Terrain::Terrain(int width, int height) :
    _program(VERT_SHADER, TESC_SHADER, TESE_SHADER, FRAG_SHADER),
    _vbo(GL_ARRAY_BUFFER),
    _ebo(GL_ELEMENT_ARRAY_BUFFER),
    _map(width, height),
    _normal_tex(GL_TEXTURE1),
    _grass_tex(GL_TEXTURE2),
    _stone_tex(GL_TEXTURE3) {
    init_buffers(width, height);
    vertex_attrib();
    set_static_uniforms();
}

void Terrain::render(glm::mat4 view, glm::mat4 proj, glm::vec3 sunPos) {
    _program.use();
    _vao.bind();

    _height_tex.bind();
    _normal_tex.bind();
    _grass_tex.bind();
    _stone_tex.bind();

    auto model_mat = glm::mat4(1);
    // model_mat = glm::scale(model_mat, glm::vec3(0.2, 0.2, 0.2));
    model_mat = glm::translate(model_mat, glm::vec3(-128, 0, -128));
    auto model_loc = _program.uniform_loc("model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_mat));

    auto view_loc = _program.uniform_loc("view");
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));

    auto proj_loc = _program.uniform_loc("proj");
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj));

    auto sun_loc = _program.uniform_loc("sunPos");
    glUniform3f(sun_loc, sunPos.x, sunPos.y, sunPos.z);

    glDrawElements(GL_PATCHES, _triangle_cnt, GL_UNSIGNED_INT, 0);
}

void Terrain::gen() {
    _vao.bind();
    _vbo.bind();
    _vbo.set(_map.vertices(_vert_factor));
}

void Terrain::update() {
    if (!_update) {
        return;
    }
    _update = false;
    _map.gen(_noise);
    gen_height_tex();
    gen_normal_tex();
    gen_noise_tex();
    gen();
}

void Terrain::set_noise(height_map::NoiseType type) {
    if (type == _noise) {
        return;
    }
    _noise = type;
    _update = true;
}

void Terrain::init_buffers(int width, int height) {
    std::cout << "Noise start" << std::endl;
    _map.gen(_noise, 15);
    std::cout << "Noise end" << std::endl;
    // _map.hydro_erosion();

    gen();
    _ebo.bind();
    auto indices = _map.indices(_vert_factor);
    _triangle_cnt = indices.size();
    _ebo.set(indices);

    init_texture(_height_tex);
    gen_height_tex();

    init_texture(_normal_tex);
    gen_normal_tex();

    init_texture(_noise_tex);
    GLint swizzle[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
    glTexParameteriv(_height_tex.kind(), GL_TEXTURE_SWIZZLE_RGBA, swizzle);
    gen_noise_tex();

    glPatchParameteri(GL_PATCH_VERTICES, 4);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    load_texture(_grass_tex, "assets/grass/Grass005_1K-JPG_Color.jpg");
    load_texture(_stone_tex, "assets/stone/Rock030_1K-JPG_Color.jpg");
}

void Terrain::vertex_attrib() {
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, sizeof(height_map::Vertex), (void *)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(height_map::Vertex),
        (void *)(sizeof(glm::vec3))
    );
    glEnableVertexAttribArray(1);
}

void Terrain::init_texture(gl::Texture &tex) {
    tex.bind();
    tex.param(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    tex.param(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    tex.param(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    tex.param(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Terrain::load_texture(gl::Texture &tex, const char *path) {
    tex.bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int w, h, channels;

    auto data = stbi_load(path, &w, &h, &channels, 0);
    if (data) {
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
}

void Terrain::gen_height_tex() {
    _height_tex.bind();
    auto data = _map.heights();
    int w = _map.width(), h = _map.height();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        _height_tex.kind(), 0, GL_R32F, w, h, 0, GL_RED, GL_FLOAT, data.data()
    );
}

void Terrain::gen_normal_tex() {
    _normal_tex.bind();
    auto data = _map.normals();
    int w = _map.width(), h = _map.height();
    glTexImage2D(
        _normal_tex.kind(),
        0,
        GL_RGB32F,
        w,
        h,
        0,
        GL_RGB,
        GL_FLOAT,
        data.data()
    );
}

void Terrain::gen_noise_tex() {
    _noise_tex.bind();
    auto data = _map.pixels();
    int w = _map.width(), h = _map.height();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(
        _noise_tex.kind(),
        0,
        GL_RED,
        w,
        h,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        data.data()
    );
}

void Terrain::set_static_uniforms() {
    _program.use();
    auto lpos_loc = _program.uniform_loc("lightPos");
    glUniform3f(lpos_loc, 128, 128, 0);
    auto light_loc = _program.uniform_loc("lightColor");
    glUniform3f(light_loc, 1, 1, 1);
    auto tex_loc = _program.uniform_loc("tex");
    glUniform1i(tex_loc, 0);
    auto norm_loc = _program.uniform_loc("normTex");
    glUniform1i(norm_loc, 1);
    auto grass_loc = _program.uniform_loc("grassTex");
    glUniform1i(grass_loc, 2);
    auto stone_loc = _program.uniform_loc("stoneTex");
    glUniform1i(stone_loc, 3);
}

} // namespace tgl::terrain
