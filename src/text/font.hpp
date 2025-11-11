#pragma once

#include <map>

#include <ft2build.h>

#include "../gl/texture.hpp"

#include <glm/glm.hpp>
#include FT_FREETYPE_H

namespace tgl::text {

struct Character {
    glm::vec2 uv;
    glm::vec2 uv_size;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class Font {
public:
    Font(const char *path);

    gl::Texture _atlas;
    std::map<char, Character> _chars;

private:
    void generate(FT_Face face);
};

} // namespace tgl::text
