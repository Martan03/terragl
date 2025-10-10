#pragma once

#include <format>
#include <functional>
#include <vector>

#include <glad/gl.h>

namespace tgl::gl {

template<typename Derived> class GLResource {
public:
    ~GLResource() {
        if (_id != 0) {
            static_cast<Derived *>(this)->destroy();
            _id = 0;
        }
    }

    GLuint get() const { return _id; }

protected:
    GLuint _id = 0;
};

static inline void check_status(
    const std::function<void(GLint, GLint, GLint *)> &get_info,
    const std::function<void(GLint, GLint, GLint *, GLchar *)> &get_log,
    GLuint id,
    GLint status,
    const char *err_msg
) {
    GLint success;
    get_info(id, status, &success);
    if (success == GL_FALSE) {
        GLint len;
        get_info(id, GL_INFO_LOG_LENGTH, &len);
        std::vector<char> msg(len + 1);
        get_log(id, len, nullptr, msg.data());
        throw std::runtime_error(std::format("{}: {}.", err_msg, msg.data()));
    }
}

} // namespace tgl::gl
