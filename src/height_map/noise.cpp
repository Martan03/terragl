#include "noise.hpp"

#include <cmath>

namespace tgl::height_map {

float Noise::fbm(float x, float y, int oct) {
    float total = 0, max = 0;
    float freq = 1, amp = 1;

    for (int i = 0; i < oct; ++i) {
        total += noise(x * freq, y * freq) * amp;
        max += amp;

        amp *= _persistance;
        freq *= _lacunarity;
    }

    return total / max;
}

float Noise::fbm(float x, float y, float rot, int oct) {
    float total = 0, max = 0;
    float amp = 1;

    const float c = std::cos(rot);
    const float s = std::sin(rot);

    for (int i = 0; i < oct; ++i) {
        total += noise(x, y) * amp;
        max += amp;

        amp *= _persistance;

        float nx = x * c - y * s;
        float ny = x * s + y * c;
        x = nx * _lacunarity;
        y = ny * _lacunarity;
    }

    return total / max;
}

float Noise::ridged_fbm(float x, float y, int oct) {
    float n = noise(x, y) * 0.5 + 0.5;
    float total = n, max = 1;
    float freq = _lacunarity, amp = _persistance * _persistance;

    for (int i = 0; i < oct; ++i) {
        float n = 1.0f - std::fabs(noise(x * freq, y * freq));
        n *= total;
        total += n * amp;
        max += amp * total;

        amp *= _persistance;
        freq *= _lacunarity;
    }

    return total / max * 2 - 1;
}

} // namespace tgl::height_map
