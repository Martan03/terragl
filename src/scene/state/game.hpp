#pragma once

#include "../../gl/camera.hpp"
#include "../../gui/quad/quad.hpp"
#include "../../sky/sky.hpp"
#include "../../terrain/terrain.hpp"
#include "state.hpp"

#include <glm/ext/matrix_clip_space.hpp>

namespace tgl::scene::state {

class Game : public State {
public:
    Game(Scene &scene);

    void render() override;

    void resize() override;

    void handle_input(float delta) override;
    void handle_key(int key, int scancode, int action, int mods) override;

    void handle_mouse(float xpos, float ypos) override;
    void handle_scroll(float xoff, float yoff) override;

    void handle_controllers(float delta) override;

    terrain::Terrain &terrain() { return _terrain; }

private:
    gl::Camera _camera;
    terrain::Terrain _terrain;
    sky::Sky _sky;
    gui::Quad _map;

    float _last_x = -1;
    float _last_y = -1;
    float _scroll_y = 0;
};

} // namespace tgl::scene::state
