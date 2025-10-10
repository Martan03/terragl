#pragma once

#include <glad/gl.h>

#include "helper.hpp"

namespace tgl::gl {

class Texture : public GLResource<Texture> {
public:
    Texture() { glGenTextures(1, &_id); }

    void bind() { glBindTexture(GL_TEXTURE_2D, _id); }
    void destroy() { glDeleteTextures(1, &_id); }
};

} // namespace tgl::gl
