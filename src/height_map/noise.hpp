#pragma once

namespace tgl::height_map {

class Noise {
public:
    Noise(float lacunarity = 2, float persist = 0.5) :
        _lacunarity(lacunarity), _persistance(persist) { }

    virtual ~Noise() = default;

    virtual float noise(float x, float y) = 0;

    float fbm(float x, float y, int oct);

protected:
    // Multiplies frequency each fbm iteration
    float _lacunarity;
    // Multiplies amplitude each fbm iteration
    float _persistance;
};

} // namespace tgl::height_map
