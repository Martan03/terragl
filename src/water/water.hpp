#pragma once

#include "../gl/camera.hpp"
#include "../gl/program.hpp"
#include "../gl/texture.hpp"
#include "../terrain/terrain.hpp"

#include <glm/ext/vector_float3.hpp>

namespace tgl::water {

class Water {
public:
    Water(gl::Camera &camera, terrain::Terrain &terrain);

    void render(glm::mat4 view, glm::mat4 proj, glm::vec3 sunPos, float time);
    void update(float delta);

    void regenerate();

    gl::Texture &texture() { return _tex; }

private:
    gl::Camera &_camera;
    terrain::Terrain &_terrain;

    gl::Program _program;
    gl::Texture _tex;

    float _offset = 0;

    void init();
    void gen_tex();
};

} // namespace tgl::water
