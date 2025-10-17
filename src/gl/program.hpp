#pragma once

#include <glad/gl.h>

#include "helper.hpp"
#include "shader.hpp"

namespace tgl::gl {

DEL_GL_STRUCT(Program, glDeleteProgram);

class Program : public GLResource<del::Program> {
public:
    Program(const char *ver_shader, const char *frag_shader) {
        _id = glCreateProgram();

        link_shader(ver_shader, GL_VERTEX_SHADER);
        link_shader(frag_shader, GL_FRAGMENT_SHADER);

        link_program();
    }

    Program(
        const char *ver_shader,
        const char *tesc_shader,
        const char *tese_shader,
        const char *frag_shader
    ) {
        _id = glCreateProgram();

        link_shader(ver_shader, GL_VERTEX_SHADER);
        link_shader(tesc_shader, GL_TESS_CONTROL_SHADER);
        link_shader(tese_shader, GL_TESS_EVALUATION_SHADER);
        link_shader(frag_shader, GL_FRAGMENT_SHADER);

        link_program();
    }

    void use() const { glUseProgram(_id); }

    GLint uniform_loc(const char *name) const {
        return glGetUniformLocation(_id, name);
    }

private:
    void link_shader(const char *data, int type) {
        auto shader = Shader(data, type);
        shader.compile();
        glAttachShader(_id, shader.get());
    }

    void link_program() {
        glLinkProgram(_id);
        check_status(
            glGetProgramiv,
            glGetProgramInfoLog,
            _id,
            GL_LINK_STATUS,
            "Failed to link program"
        );
    }
};

} // namespace tgl::gl
