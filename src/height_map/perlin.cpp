#include "perlin.hpp"

#include <cmath>

namespace tgl::height_map {

Perlin::Perlin() : Noise() { }

float Perlin::noise(float x, float y) {
    float fx = std::floor(x);
    float fy = std::floor(y);

    int xi = (int)fx & 255;
    int yi = (int)fy & 255;

    x -= fx;
    y -= fy;

    float u = fade(x), v = fade(y);

    int a = _perm[xi] + yi;
    int b = _perm[xi + 1] + yi;
    int aa = _perm[a], ab = _perm[a + 1];
    int ba = _perm[b], bb = _perm[b + 1];

    float x1 = lerp(grad(aa, x, y), grad(ba, x - 1, y), u);
    float x2 = lerp(grad(ab, x, y - 1), grad(bb, x - 1, y - 1), u);
    return lerp(x1, x2, v);
}

glm::vec3 Perlin::deriv_noise(float x, float y) {
    int xi = (int)std::floor(x) & 255;
    int yi = (int)std::floor(y) & 255;

    x -= std::floor(x);
    y -= std::floor(y);

    float u = fade(x), v = fade(y);
    float du = d_fade(x), dv = d_fade(y);

    int a = _perm[xi] + yi;
    glm::vec2 g00 = get_grad(_perm[a]);
    glm::vec2 g01 = get_grad(_perm[a + 1]);
    float p00 = g00.x * x + g00.y * y;
    float p01 = g01.x * x + g01.y * (y - 1);

    int b = _perm[xi + 1] + yi;
    glm::vec2 g10 = get_grad(_perm[b]);
    glm::vec2 g11 = get_grad(_perm[b + 1]);
    float p10 = g10.x * (x - 1) + g10.y * y;
    float p11 = g11.x * (x - 1) + g11.y * (y - 1);

    float x1 = lerp(p00, p10, u);
    float x2 = lerp(p01, p11, u);
    float val = lerp(x1, x2, v);

    float gx1 = lerp(g00.x, g10.x, u) + du * (p10 - p00);
    float gx2 = lerp(g01.x, g11.x, u) + du * (p11 - p01);
    float dx = lerp(gx1, gx2, v);

    float gy1 = lerp(g00.y, g10.y, u);
    float gy2 = lerp(g01.y, g11.y, u);
    float dy = lerp(gy1, gy2, v) + dv * (x2 - x1);

    return glm::vec3{ dx, dy, val };
}

} // namespace tgl::height_map
