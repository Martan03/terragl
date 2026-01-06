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
        glActiveTexture(unit);
        glBindTexture(_kind, _id);
    }

    void param(GLenum name, GLenum val) const {
        glTexParameteri(_kind, name, val);
    }

    /// Sets both min and mag filter of the texture to given type
    void filter(GLenum type) {
        param(GL_TEXTURE_MIN_FILTER, type);
        param(GL_TEXTURE_MAG_FILTER, type);
    }

    /// Sets both wrap S and wrap T of the texture to given type
    void wrap(GLenum type) {
        param(GL_TEXTURE_WRAP_S, type);
        param(GL_TEXTURE_WRAP_T, type);
    }

private:
    GLenum _kind = GL_TEXTURE_2D;
    GLenum _unit = GL_TEXTURE0;
};

} // namespace tgl::gl
