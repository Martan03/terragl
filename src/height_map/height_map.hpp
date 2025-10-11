#pragma once

#include <vector>

#include "perlin2.hpp"

#include <glm/ext/vector_float3.hpp>

namespace tgl::height_map {

class HeightMap {
public:
    HeightMap(
        int width,
        int height,
        float freq = 0.05,
        float amp = 20,
        float lacunarity = 2,
        float persist = 0.5
    ) :
        _width(width),
        _height(height),
        _map(width * height),
        _freq(freq),
        _amp(amp),
        _lacunarity(lacunarity),
        _persist(persist) { }

    void perlin_gen(int oct = 1) {
        auto perlin = Perlin2();
        for_each([&](int x, int y, int id) {
            float val = perlin.noise(x * _freq, y * _freq, oct);
            _map[id] = (val + 1) * 0.5f;
        });
    }

    std::vector<glm::vec3> vertices() {
        std::vector<glm::vec3> vertices;
        for_each([&](int x, int y, int id) {
            vertices.push_back(glm::vec3(x, _map[id], y));
        });
        return vertices;
    }

    std::vector<unsigned int> indices() {
        std::vector<unsigned int> indices;
        for (int y = 0; y < _height - 1; ++y) {
            for (int x = 0; x < _width - 1; ++x) {
                int id = y * _width + x;

                indices.push_back(id);
                indices.push_back(id + 1);
                indices.push_back(id + _width);

                indices.push_back(id + 1);
                indices.push_back(id + _width + 1);
                indices.push_back(id + _width);
            }
        }
        return indices;
    }

    std::vector<unsigned char> pixels() {
        auto cnt = _width * _height;
        std::vector<unsigned char> pixels(cnt);
        for (int i = 0; i < cnt; ++i) {
            pixels[i] = static_cast<unsigned char>(_map[i] * 255.0f);
        }
        return pixels;
    }

private:
    int _width, _height;
    float _freq = 0.05, _amp = 20;
    float _lacunarity = 2, _persist = 0.5;
    std::vector<float> _map;

    template<typename Func> void for_each(Func func) {
        for (int y = 0; y < _height; ++y) {
            for (int x = 0; x < _width; ++x) {
                auto id = y * _width + x;
                func(x, y, id);
            }
        }
    }
};

} // namespace tgl::height_map
