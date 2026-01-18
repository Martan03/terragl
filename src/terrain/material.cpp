#include "material.hpp"

#include "stb_image.h"

namespace tgl::terrain {

Material::Material(GLenum unit) :
    _albedo(unit), _normal(unit + 1), _pbr(unit + 2) { }

void Material::load(
    const char *albedo,
    const char *normal,
    const char *ao,
    const char *rough,
    const char *disp
) {
    load_texture(_albedo, albedo, true);
    load_texture(_normal, normal);

    int w, h, ch;
    auto ao_data = stbi_load(ao, &w, &h, &ch, 1);
    auto rough_data = stbi_load(rough, &w, &h, &ch, 1);
    auto disp_data = stbi_load(disp, &w, &h, &ch, 1);

    if (ao_data && rough_data && disp_data) {
        std::vector<unsigned char> packed(w * h * 3);
        for (int i = 0; i < w * h; i += 1) {
            packed[i * 3] = ao_data[i];
            packed[i * 3 + 1] = rough[i];
            packed[i * 3 + 2] = disp[i];
        }
        auto d = packed.data();
        _pbr.bind();
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, d
        );
    }
    stbi_image_free(ao_data);
    stbi_image_free(rough_data);
    stbi_image_free(disp_data);
}

void Material::load_texture(gl::Texture &tex, const char *path, bool color) {
    tex.bind();
    tex.wrap(GL_REPEAT);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int w, h, channels;
    auto data = stbi_load(path, &w, &h, &channels, 0);
    if (data) {
        GLint format = color ? GL_SRGB : GL_RGB;
        glTexImage2D(
            GL_TEXTURE_2D, 0, format, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data
        );
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
}

} // namespace tgl::terrain
