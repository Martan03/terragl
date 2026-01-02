#pragma once

#include <glad/gl.h>

#include "helper.hpp"

namespace tgl::gl {

DEL_GLS_STRUCT(Texture, glDeleteTextures);

class Texture : public GLResource<del::Texture> {
public:
    Texture() { glGenTextures(1, &_id); }

    Texture(GLenum unit, GLenum kind = GL_TEXTURE_2D) :
        _kind(kind), _unit(unit) {
        glGenTextures(1, &_id);
    }

    GLenum kind() { return _kind; };

    void bind() const {
        glActiveTexture(_unit);
        glBindTexture(_kind, _id);
    }
    void bind(GLenum unit) {
        _unit = unit;
        bind();
    }

    void param(GLenum name, GLenum val) const {
        glTexParameteri(_kind, name, val);
    }

private:
    GLenum _kind = GL_TEXTURE_2D;
    GLenum _unit = GL_TEXTURE0;
};

} // namespace tgl::gl
