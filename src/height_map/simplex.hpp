#pragma once

#include <cmath>
#include <cstdint>

#include "noise.hpp"

#include <glm/glm.hpp>

namespace tgl::height_map {

class Simplex : public Noise {
public:
    Simplex();

    float noise(float x, float y) override;
    glm::vec3 deriv_noise(float x, float y) override;

private:
    const float _f2 = (std::sqrt(3) - 1) / 2;
    const float _g2 = (1 - 1 / std::sqrt(3)) / 2;

    static inline uint8_t hash(int i);
    static float grad(int hash, float x, float y);
    static inline glm::vec3 proc_corner(float x, float y, int hash);

    static constexpr float GRAD3[8][2] = {
        { 1, 2 }, { -1, 2 }, { 1, -2 }, { -1, -2 }, // h < 4
        { 2, 1 }, { -2, 1 }, { 2, -1 }, { -2, -1 }  // h >= 4
    };
};

} // namespace tgl::height_map
