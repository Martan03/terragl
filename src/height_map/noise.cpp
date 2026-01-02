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

glm::vec3 Noise::deriv_fbm(float x, float y, int oct) {
    const glm::mat2 rot = glm::mat2(0.87758f, 0.47946f, -0.47946f, 0.87758f);

    float height = 0.0f;
    glm::vec2 grad_sum = glm::vec2(0.0f);
    glm::vec2 d_hist = glm::vec2(0.0f);

    float amp = 1.0f, freq = 1.0f;
    float max_amp = 0.0f;

    glm::vec2 p(x, y);
    for (int i = 0; i < oct; i++) {
        glm::vec3 n = deriv_noise(p.x, p.y);

        d_hist += glm::vec2(n.x, n.y);

        float weight = 1.0f / (1.0f + glm::dot(d_hist, d_hist));

        // if (i > 4)
        //     weight = 1.0f;

        float signal = n.z * weight;
        height += signal * amp;
        max_amp += amp;

        grad_sum += glm::vec2(n.x, n.y) * weight * amp * freq;

        amp *= 0.5f;
        freq *= 2.0f;
        p = rot * p * 2.0f;
    }

    return glm::vec3(grad_sum.x, grad_sum.y, height);
}

float Noise::ridged_fbm(float x, float y, int oct) {
    float n = deriv_noise(x, y).z * 0.5f + 0.5f;
    float total = n, max = 1;
    float freq = _lacunarity, amp = _persistance * _persistance;

    for (int i = 1; i < oct; ++i) {
        float n = 1.0f - std::fabs(deriv_noise(x * freq, y * freq).z);
        n *= total;
        total += n * amp;
        max += amp * total;

        amp *= _persistance;
        freq *= _lacunarity;
    }

    return total / max * 2 - 1;
}

float Noise::ridged_fbm(float x, float y, float rot, int oct) {
    float n = noise(x, y) * 0.5f + 0.5f;
    float total = n, max = 1;
    float freq = _lacunarity, amp = _persistance * _persistance;

    const float c = std::cos(rot);
    const float s = std::sin(rot);

    for (int i = 1; i < oct; ++i) {
        n = 1.0f - std::fabs(noise(x + 173.36, y + 93.63));
        n *= total * amp;
        total += n;
        max += amp * total;

        amp *= _persistance;

        float nx = x * c - y * s;
        float ny = x * s + y * c;
        x = nx * _lacunarity;
        y = ny * _lacunarity;
    }

    return total / max * 2 - 1;
}

} // namespace tgl::height_map
