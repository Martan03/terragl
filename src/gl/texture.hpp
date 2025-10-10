#pragma once

#include <glad/gl.h>

#include "helper.hpp"

namespace tgl::gl {

class Texture : public GLResource<Texture> {
public:
    Texture(GLenum kind = GL_TEXTURE_2D) : _kind(kind) {
        glGenTextures(1, &_id);
    }

    void bind() { glBindTexture(_kind, _id); }
    void destroy() { glDeleteTextures(1, &_id); }

    void param(GLenum name, GLenum val) { glTexParameteri(_kind, name, val); }

private:
    GLenum _kind;
};

} // namespace tgl::gl
