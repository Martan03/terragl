#pragma once

#include <glad/gl.h>

#include "helper.hpp"
#include "shader.hpp"

namespace tgl::gl {

class Program : public GLResource<Program> {
public:
    Program(const char *ver_shader, const char *frag_shader) {
        auto vertex = Shader::vertex(ver_shader);
        vertex.compile();

        auto fragment = Shader::fragment(frag_shader);
        fragment.compile();

        _id = glCreateProgram();
        glAttachShader(_id, vertex.get());
        glAttachShader(_id, fragment.get());

        glLinkProgram(_id);
        check_status(
            glGetProgramiv,
            glGetProgramInfoLog,
            _id,
            GL_LINK_STATUS,
            "Failed to link program"
        );
    }

    void use() const { glUseProgram(_id); }
    void destroy() const { glDeleteProgram(_id); }

    GLint uniform_loc(const char *name) const {
        return glGetUniformLocation(_id, name);
    }
};

} // namespace tgl::gl
