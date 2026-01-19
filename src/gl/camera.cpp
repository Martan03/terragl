#include "camera.hpp"

namespace tgl::gl {

Camera::Camera(glm::vec3 pos) : _pos(pos) {
    update_vecs();
}

void Camera::process_move(glm::vec2 move, float delta) {
    move = glm::normalize(move);
    float velocity = _speed * delta;
    if (move.y != 0)
        _pos += _dir * move.y * velocity;
    if (move.x != 0)
        _pos += glm::normalize(glm::cross(_dir, _up)) * move.x * velocity;
}

void Camera::process_fly(float move, float delta) {
    float velocity = _speed * delta;
    _pos.y += move * velocity;
}

void Camera::process_look(float xoffset, float yoffset, float sens) {
    _yaw += xoffset * sens;
    _pitch += yoffset * sens;
    _pitch = std::clamp(_pitch, -89.0f, 89.0f);
    update_vecs();
}

void Camera::update_vecs() {
    _dir.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _dir.y = sin(glm::radians(_pitch));
    _dir.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _dir = glm::normalize(_dir);

    _right = glm::normalize(glm::cross(_dir, glm::vec3(0, 1, 0)));
    _up = glm::normalize(glm::cross(_right, _dir));
}

} // namespace tgl::gl
