#include "font.hpp"

#include <algorithm>
#include <stdexcept>

#include <freetype/freetype.h>

namespace tgl::text {

Font::Font(const char *path) : _atlas() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        throw std::runtime_error("Failed to initialize FreeType library");
    }

    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face)) {
        throw std::runtime_error(std::format("Failed to load font: {}", path));
    }
    FT_Set_Pixel_Sizes(face, 0, 20);
    generate(face);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Font::generate(FT_Face face) {
    int width = 0;
    int height = 0;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (int c = 0; c < 128; ++c) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            continue;
        width += face->glyph->bitmap.width;
        height = std::max(height, int(face->glyph->bitmap.rows));
    }

    _atlas.bind();
    glTexImage2D(
        _atlas.kind(),
        0,
        GL_RED,
        width,
        height,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        nullptr
    );

    _atlas.param(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    _atlas.param(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    _atlas.param(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    _atlas.param(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int x = 0;
    for (unsigned char c = 0; c < 128; ++c) {
        // TODO: log it at least or throw exception
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            continue;

        auto bitmap = face->glyph->bitmap;
        glTexSubImage2D(
            _atlas.kind(),
            0,
            x,
            0,
            bitmap.width,
            bitmap.rows,
            GL_RED,
            GL_UNSIGNED_BYTE,
            bitmap.buffer
        );

        float u0 = float(x) / float(width);
        float v0 = 0;
        float u1 = float(x + bitmap.width) / float(width);
        float v1 = float(bitmap.rows) / float(height);
        _chars[c] = {
            glm::vec2(u0, v0),
            glm::vec2(u1 - u0, v1 - v0),
            glm::ivec2(bitmap.width, bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        x += bitmap.width;
    }
}

} // namespace tgl::text
