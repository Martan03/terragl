#include "perlin.hpp"

#include <cmath>

namespace tgl::height_map {

Perlin::Perlin() : Noise() {
    _perm.resize(512);
    for (int i = 0; i < 256; i++) {
        _perm[i] = p[i];
        _perm[i + 256] = p[i];
    }
}

float Perlin::noise(float x, float y) {
    int xi = (int)std::floor(x) & 255;
    int yi = (int)std::floor(y) & 255;

    x -= std::floor(x);
    y -= std::floor(y);

    float u = fade(x), v = fade(y);

    int a = _perm[xi] + yi;
    int aa = _perm[a], ab = _perm[a + 1];
    int b = _perm[xi + 1] + yi;
    int ba = _perm[b], bb = _perm[b + 1];

    float x1 = lerp(grad(aa, x, y), grad(ba, x - 1, y), u);
    float x2 = lerp(grad(ab, x, y - 1), grad(bb, x - 1, y - 1), u);
    return lerp(x1, x2, v);
}

} // namespace tgl::height_map
