#pragma once

#include <format>
#include <functional>
#include <utility>
#include <vector>

#include <glad/gl.h>

namespace tgl::gl {

#define DEL_GL_STRUCT(name, destroy)                                          \
    namespace del {                                                           \
    struct name {                                                             \
        void operator()(GLuint id) { destroy(id); }                           \
    };                                                                        \
    }

#define DEL_GLS_STRUCT(name, destroy)                                         \
    namespace del {                                                           \
    struct name {                                                             \
        void operator()(GLuint id) { destroy(1, &id); }                       \
    };                                                                        \
    }

template<typename Del> class GLResource {
public:
    GLResource(GLuint id = 0) : _id(id) { }

    GLResource(const GLResource<Del> &) = delete;
    GLResource(GLResource<Del> &&other) : _id(other._id) { other._id = 0; }

    GLResource<Del> &operator=(const GLResource<Del> &) = delete;
    GLResource<Del> &operator=(GLResource<Del> &&other) {
        std::swap(_id, other._id);
    }

    ~GLResource() {
        if (_id != 0) {
            Del()(_id);
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
