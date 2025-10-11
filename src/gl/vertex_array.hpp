#pragma once

#include <glad/gl.h>

#include "helper.hpp"

namespace tgl::gl {

class VertexArray : public GLResource<VertexArray> {
public:
    VertexArray() { glGenVertexArrays(1, &_id); }

    void bind() const { glBindVertexArray(_id); }
    void destroy() const { glDeleteVertexArrays(1, &_id); }
};

} // namespace tgl::gl
