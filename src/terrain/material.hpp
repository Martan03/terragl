#pragma once

#include "../gl/texture.hpp"

namespace tgl::terrain {

class Material {
public:
    // Start unit for the textures, next will use following units
    // - albedo = unit
    // - normal = unit + 1
    // - pbr = unit + 2
    Material(GLenum unit);

    /// Loads texture from given paths and joins AO, Roughness and Displacement
    /// into one texture.
    void load(
        const char *albedo,
        const char *normal,
        const char *ao,
        const char *rough,
        const char *disp
    );

    /// Binds all the material textures
    void bind();

private:
    // RGB = material colors
    gl::Texture _albedo;
    // RGB = normal vector
    gl::Texture _normal;
    // R = ambient occlusion, G = roughness, B = displacement
    gl::Texture _pbr;

    void load_texture(gl::Texture &tex, const char *path, bool color = false);
};

} // namespace tgl::terrain
