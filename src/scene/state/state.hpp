#pragma once

namespace tgl::scene {
class Scene;
}

namespace tgl::scene::state {

class State {
public:
    State(Scene &scene) : _scene(scene) { }
    virtual ~State() = default;

    virtual void render() = 0;

    virtual void handle_input(float delta) = 0;

    virtual void handle_mouse(float xpos, float ypos) { };
    virtual void handle_click(int button, int action, int mods) { };
    virtual void handle_scroll(float xoff, float yoff) { };

    virtual void handle_controllers(float delta) { };

protected:
    Scene &_scene;
};

} // namespace tgl::scene::state
