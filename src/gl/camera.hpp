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

const float SPEED = 20;
const float SENS = 0.1;
const float CONT_SENS = 1;

class Camera {
public:
    Camera(glm::vec3 pos) {
        _pos = pos;
        update_vecs();
    }

    glm::mat4 view() { return glm::lookAt(_pos, _pos + _dir, _up); }

    void process_move(glm::vec2 move, float delta) {
        move = glm::normalize(move);
        float velocity = _speed * delta;
        if (move.y != 0)
            _pos += _dir * move.y * velocity;
        if (move.x != 0)
            _pos += glm::normalize(glm::cross(_dir, _up)) * move.x * velocity;
    }

    void process_mouse(float xoffset, float yoffset) {
        process_look(xoffset, yoffset, SENS);
    }

    void process_controller(float xoffset, float yoffset) {
        process_look(xoffset, yoffset, CONT_SENS);
    }

    void process_scroll(float yoffset, float sens = SENS) {
        _speed = std::max(_speed + SPEED * yoffset * sens, 1.0f);
    }

private:
    GLfloat _yaw = -90;
    GLfloat _pitch = -25;

    GLfloat _speed = SPEED;

    glm::vec3 _pos;
    glm::vec3 _dir = glm::vec3(0, 0, -1);
    glm::vec3 _right;
    glm::vec3 _up = glm::vec3(0, 1, 0);

    void process_look(float xoffset, float yoffset, float sens) {
        _yaw += xoffset * sens;
        _pitch += yoffset * sens;
        _pitch = std::clamp(_pitch, -89.0f, 89.0f);
        update_vecs();
    }

    void update_vecs() {
        _dir.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        _dir.y = sin(glm::radians(_pitch));
        _dir.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
        _dir = glm::normalize(_dir);

        _right = glm::normalize(glm::cross(_dir, glm::vec3(0, 1, 0)));
        _up = glm::normalize(glm::cross(_right, _dir));
    }
};

} // namespace tgl::gl
