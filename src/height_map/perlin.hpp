#pragma once

#include "noise.hpp"

#include <glm/glm.hpp>

namespace tgl::height_map {

class Perlin : public Noise {
public:
    Perlin();

    float noise(float x, float y) override;
    glm::vec3 deriv_noise(float x, float y) override;

private:
    static float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
    static float d_fade(float t) {
        return 30.0f * t * t * (t * (t - 2.0f) + 1.0f);
    }

    static float lerp(float a, float b, float t) { return a + t * (b - a); }

    static float grad(int hash, float x, float y) {
        auto g = get_grad(hash);
        return g.x * x + g.y * y;
    }
    static glm::vec2 get_grad(int hash) {
        static const int grad2[8][2] = { { 1, 0 },  { -1, 0 }, { 0, 1 },
                                         { 0, -1 }, { 1, 1 },  { -1, 1 },
                                         { 1, -1 }, { -1, -1 } };
        const int *g = grad2[hash & 7];
        return { g[0], g[1] };
    }
};

} // namespace tgl::height_map
