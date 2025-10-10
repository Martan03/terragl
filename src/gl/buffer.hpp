#pragma once

#include <glad/gl.h>

namespace tgl::gl {

class Buffer {
public:
    Buffer() { glGenBuffers(1, &this->id); }

    ~Buffer() {
        if (this->id != 0) {
            glDeleteBuffers(1, &this->id);
            this->id = 0;
        }
    }

    GLuint get() { return this->id; }
    void bind(GLenum target) { glBindBuffer(target, this->id); }

private:
    GLuint id = 0;
};

} // namespace tgl::gl
