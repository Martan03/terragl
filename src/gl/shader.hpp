#pragma once

#include <glad/gl.h>

#include "helper.hpp"

namespace tgl::gl {

class Shader {
public:
    Shader(const char *shader, int type) {
        this->id = glCreateShader(type);
        glShaderSource(this->id, 1, &shader, nullptr);
    }

    static Shader vertex(const char *shader) {
        return { shader, GL_VERTEX_SHADER };
    }

    static Shader fragment(const char *shader) {
        return { shader, GL_FRAGMENT_SHADER };
    }

    ~Shader() {
        if (this->id != 0) {
            glDeleteShader(this->id);
            this->id = 0;
        }
    }

    GLuint get() { return this->id; }

    void compile() {
        glCompileShader(this->id);
        check_status(
            glGetShaderiv,
            glGetShaderInfoLog,
            this->id,
            GL_COMPILE_STATUS,
            "Failed to compile shader"
        );
    }

private:
    GLuint id = 0;
};

} // namespace tgl::gl
