#pragma once

#include "../gl/buffer.hpp"
#include "../gl/program.hpp"
#include "../gl/texture.hpp"
#include "../gl/vertex_array.hpp"
#include "../height_map/map.hpp"

#include <glm/glm.hpp>

namespace tgl::terrain {

class Terrain {
public:
    Terrain(int width, int height);

    void render(glm::mat4 view, glm::mat4 proj);

    void gen();
    void update();

    void set_noise(height_map::NoiseType type);

    height_map::Map &map() { return _map; }
    gl::Texture &texture() { return _noise_tex; }
    gl::Texture &normal_texture() { return _normal_tex; }

private:
    gl::Program _program;
    gl::VertexArray _vao;
    gl::Buffer _vbo;
    gl::Buffer _ebo;

    height_map::NoiseType _noise = height_map::NoiseType::Perlin;

    height_map::Map _map;
    gl::Texture _height_tex;
    gl::Texture _normal_tex;
    gl::Texture _noise_tex;

    gl::Texture _grass_tex;
    gl::Texture _stone_tex;

    bool _update = false;
    unsigned int _triangle_cnt = 0;

    int _vert_factor = 4;

    void init_buffers(int width, int height);
    void vertex_attrib();

    void init_texture(gl::Texture &tex);
    void load_texture(gl::Texture &tex, const char *path);

    void gen_height_tex();
    void gen_normal_tex();
    void gen_noise_tex();

    void set_static_uniforms();
};

} // namespace tgl::terrain
