#pragma once

#include "../gl/program.hpp"
#include "../gl/texture.hpp"
#include "../terrain/terrain.hpp"

#include <glm/ext/vector_float3.hpp>

namespace tgl::water {

class Water {
public:
    Water(terrain::Terrain &terrain);

    void render(glm::mat4 view, glm::mat4 proj, glm::vec3 sunPos, float time);

    gl::Texture &texture() { return _tex; }

private:
    terrain::Terrain &_terrain;

    gl::Program _program;
    gl::Texture _tex;

    void init();
    void gen_tex();
};

} // namespace tgl::water
