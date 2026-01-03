#pragma once

#include "../gl/buffer.hpp"
#include "../gl/program.hpp"
#include "../gl/vertex_array.hpp"

#include <glm/glm.hpp>

namespace tgl::sky {

class Sky {
public:
    glm::vec3 sunPos;

    Sky();

    void render(glm::mat4 view, glm::mat4 proj);
    void update(float delta);

    void toggleDayCycle() { _dayCycle = !_dayCycle; }

private:
    gl::Program _program;
    gl::VertexArray _vao;
    gl::Buffer _vbo;
    gl::Buffer _ebo;

    float _dayTime = 0.0f;
    float _daySpeed = 0.1f;
    bool _dayCycle = true;

    void init();
};

} // namespace tgl::sky
