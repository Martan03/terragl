#pragma once

#include <vector>

#include "perlin2.hpp"
namespace tgl::height_map {

class HeightMap {
public:
    HeightMap(int width, int height, float freq = 0.05, float amp = 20) :
        _width(width),
        _height(height),
        _map(width * height),
        _freq(freq),
        _amp(amp) { }

    void perlin_gen() {
        auto perlin = Perlin2();
        for (int y = 0; y < _height; ++y) {
            for (int x = 0; x < _width; ++x) {
                float val = perlin.noise(x * _freq, y * _freq);
                _map[y * _width + x] = val * _amp;
            }
        }
    }

private:
    int _width, _height;
    float _freq, _amp;
    std::vector<float> _map;
};

} // namespace tgl::height_map
