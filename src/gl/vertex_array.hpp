#pragma once

#include <glad/gl.h>

#include "helper.hpp"

namespace tgl::gl {

DEL_GLS_STRUCT(VertexArray, glDeleteVertexArrays);

class VertexArray : public GLResource<del::VertexArray> {
public:
    VertexArray() { glGenVertexArrays(1, &_id); }

    void bind() const { glBindVertexArray(_id); }
};

} // namespace tgl::gl
