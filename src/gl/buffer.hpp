#pragma once

#include <vector>

#include <glad/gl.h>

#include "helper.hpp"

namespace tgl::gl {

DEL_GLS_STRUCT(Buffer, glDeleteBuffers);

class Buffer : public GLResource<del::Buffer> {
public:
    Buffer(GLenum target) : _kind(target) { glGenBuffers(1, &_id); }

    void bind() const { glBindBuffer(_kind, _id); }

    /// Expects the buffer is already binded
    template<typename T>
    void set(const std::vector<T> &data, GLenum usage = GL_STATIC_DRAW) const {
        glBufferData(_kind, data.size() * sizeof(T), data.data(), usage);
    }

    /// Expects the buffer is already binded
    template<typename T, size_t N>
    void set(const T (&data)[N], GLenum usage = GL_STATIC_DRAW) const {
        glBufferData(_kind, sizeof(data), data, usage);
    }

private:
    GLenum _kind;
};

} // namespace tgl::gl
