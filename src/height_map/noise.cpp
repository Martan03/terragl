#include "noise.hpp"

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

} // namespace tgl::height_map
