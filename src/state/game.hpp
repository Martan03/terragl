#pragma once

#include "../gl/camera.hpp"
#include "../terrain/terrain.hpp"
#include "state.hpp"

#include <glm/ext/matrix_clip_space.hpp>

namespace tgl::state {

class Game : public State {
public:
    Game(Scene &scene, gl::Window &window);

    void handle_input(float delta) override;
    void handle_mouse(double xpos, double ypos) override;

    void render() override;

private:
    gl::Camera _camera;
    terrain::Terrain _terrain;

    float _last_x = -1;
    float _last_y = -1;
    float _scroll_y = 0;
};

} // namespace tgl::state
