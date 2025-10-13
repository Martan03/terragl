#pragma once

#include <algorithm>

#include "glad/gl.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
namespace tgl::gl {

enum CameraMove { FORWARD, BACKWARD, LEFT, RIGHT };

class Camera {
public:
    Camera(glm::vec3 pos, glm::vec3 dir) {
        _pos = pos;
        _dir = dir;
        update_vecs();
    }

    glm::mat4 view() { return glm::lookAt(_pos, _pos + _dir, _up); }

    void process_key(CameraMove move, float delta) {
        float velocity = _speed * delta;
        if (move == FORWARD)
            _pos += _dir * velocity;
        if (move == BACKWARD)
            _pos -= _dir * velocity;
        if (move == LEFT)
            _pos -= glm::normalize(glm::cross(_dir, _up)) * velocity;
        if (move == RIGHT)
            _pos += glm::normalize(glm::cross(_dir, _up)) * velocity;
    }

    void process_mouse(float xoffset, float yoffset) {
        _yaw += xoffset * _sens;
        _pitch += std::min(std::max(yoffset * _sens, -89.0f), 89.0f);
        update_vecs();
    }

private:
    GLfloat _yaw = -90;
    GLfloat _pitch = 0;

    GLfloat _speed = 2.5;
    GLfloat _sens = 0.1;

    glm::vec3 _pos;
    glm::vec3 _dir;
    glm::vec3 _right;
    glm::vec3 _up = glm::vec3(0, 1, 0);

    void update_vecs() {
        _dir.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        _dir.y = sin(glm::radians(_pitch));
        _dir.z = sin(glm::radians(_yaw) * cos(glm::radians(_pitch)));
        _dir = glm::normalize(_dir);

        _right = glm::normalize(glm::cross(_dir, _up));
        _up = glm::normalize(glm::cross(_right, _dir));
    }
};

} // namespace tgl::gl
