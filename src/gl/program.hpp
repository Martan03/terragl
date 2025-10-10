#pragma once

#include <glad/gl.h>

#include "helper.hpp"
#include "shader.hpp"

namespace tgl::gl {

class Program {
public:
    Program(const char *ver_shader, const char *frag_shader) {
        auto vertex = Shader::vertex(ver_shader);
        vertex.compile();

        auto fragment = Shader::fragment(frag_shader);
        fragment.compile();

        this->id = glCreateProgram();
        glAttachShader(this->id, vertex.get());
        glAttachShader(this->id, fragment.get());

        glLinkProgram(this->id);
        check_status(
            glGetProgramiv,
            glGetProgramInfoLog,
            this->id,
            GL_LINK_STATUS,
            "Failed to link program"
        );
    }

    ~Program() {
        if (this->id != 0) {
            glDeleteProgram(this->id);
            this->id = 0;
        }
    }

    void use() { glUseProgram(this->id); }

private:
    GLuint id = 0;
};

} // namespace tgl::gl
