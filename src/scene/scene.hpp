#pragma once

#include <unordered_map>
#include <unordered_set>

#include <glad/gl.h>

#include "../gl/camera.hpp"
#include "../gl/window.hpp"
#include "../terrain/terrain.hpp"
#include "state/state.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#if true
#include <GLFW/glfw3.h>
#endif

namespace tgl::scene {

enum StateType {
    Game,
    Menu,
};

class Scene {
public:
    Scene(gl::Window win, glm::vec3 cam);

    void main_loop();

    gl::Window &window() { return _window; }
    std::unordered_set<int> &controllers() { return _controllers; }

    state::State *state(StateType type) { return _states.at(type).get(); }
    void set_state(StateType type);

private:
    static inline Scene *_singleton = nullptr;

    tgl::gl::Window _window;
    tgl::gl::Camera _camera;
    tgl::terrain::Terrain _terrain;

    std::unordered_map<StateType, std::unique_ptr<state::State>> _states;
    state::State *_active = nullptr;

    std::unordered_set<int> _controllers;

    float _delta = 0;
    float _last = 0;

    void setup_win();

    void update_delta();

    static void handle_resize(GLFWwindow *win, int width, int height);

    static void handle_mouse(GLFWwindow *win, double xpos_in, double ypos_in);
    static void handle_scroll(GLFWwindow *win, double xoff, double yoff);

    static void handle_key(
        GLFWwindow *win, int key, int scancode, int action, int mods
    );

    void check_controllers();
    static void joystick_callback(int jid, int event);

    static inline Scene *get_context(GLFWwindow *win);
};

} // namespace tgl::scene
