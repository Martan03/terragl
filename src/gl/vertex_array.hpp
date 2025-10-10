#pragma once

#include <glad/gl.h>

namespace tgl::gl {

class VertexArray {
public:
    VertexArray() { glGenVertexArrays(1, &this->id); }

    ~VertexArray() {
        if (this->id != 0) {
            glDeleteVertexArrays(1, &this->id);
            this->id = 0;
        }
    }

    GLuint get() { return this->id; }
    void bind() { glBindVertexArray(this->id); }

private:
    GLuint id = 0;
};

} // namespace tgl::gl
