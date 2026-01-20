#pragma once

#include <cmath>

#include <glad/gl.h>

#if true
#include <GLFW/glfw3.h>
#endif

namespace tgl::scene {
class Scene;
} // namespace tgl::scene

namespace tgl::scene::state {

class State {
public:
    State(Scene &scene) : _scene(scene) { }
    virtual ~State() = default;

    virtual void render() = 0;
    virtual void update() { };

    virtual void resize() { };

    virtual void handle_key(int key, int scancode, int action, int mods) { };
    virtual void handle_input(float delta) { };

    virtual void handle_mouse(float xpos, float ypos) { };
    virtual void handle_click(int button, int action, int mods) { };
    virtual void handle_scroll(float xoff, float yoff) { };

    virtual void handle_controller(GLFWgamepadstate &state, int jid) { };

protected:
    Scene &_scene;

    float getGpAxes(GLFWgamepadstate &state, int key, float deadzone = 0.15f) {
        auto val = state.axes[key];
        return fabs(val) < deadzone ? 0.0f : val;
    }
};

} // namespace tgl::scene::state
