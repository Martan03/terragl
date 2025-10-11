#pragma once

#include <glad/gl.h>

#include "helper.hpp"

namespace tgl::gl {

class Shader : public GLResource<Shader> {
public:
    Shader(const char *shader, int type) {
        _id = glCreateShader(type);
        glShaderSource(_id, 1, &shader, nullptr);
    }

    static Shader vertex(const char *shader) {
        return { shader, GL_VERTEX_SHADER };
    }

    static Shader fragment(const char *shader) {
        return { shader, GL_FRAGMENT_SHADER };
    }

    void compile() const {
        glCompileShader(_id);
        check_status(
            glGetShaderiv,
            glGetShaderInfoLog,
            _id,
            GL_COMPILE_STATUS,
            "Failed to compile shader"
        );
    }

    void destroy() const { glDeleteShader(_id); }
};

} // namespace tgl::gl
