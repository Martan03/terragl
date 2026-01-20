#pragma once

#include <algorithm>

#include <glad/gl.h>

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
    Camera(glm::vec3 pos);

    glm::mat4 view() { return glm::lookAt(_pos, _pos + _dir, _up); }
    glm::vec3 &position() { return _pos; }

    /// Processes the fly-by camera movement
    void process_move(glm::vec2 move, float delta);
    /// Processes the fly-by camera height movement
    void process_fly(float move, float delta);

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

    void process_look(float xoffset, float yoffset, float sens);
    void update_vecs();
};

} // namespace tgl::gl
