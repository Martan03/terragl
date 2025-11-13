#pragma once

#include "../gl/window.hpp"

namespace tgl {
class Scene;
}

namespace tgl::state {

class State {
public:
    State(Scene &scene, gl::Window &window) :
        _scene(scene), _window(window) { }
    virtual ~State() = default;

    virtual void handle_input(float delta) = 0;
    virtual void handle_mouse(double xpos, double ypos) { };
    virtual void handle_click(int button, int action, int mods) { };
    virtual void render() = 0;

protected:
    Scene &_scene;
    gl::Window &_window;
};

} // namespace tgl::state
