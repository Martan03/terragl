#pragma once

#include <vector>

#include "perlin2.hpp"

#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>

namespace tgl::height_map {

struct ErosionConf {
    // Number of droplets to simulate
    int droplets = 100;
    // TTL of droplet (steps it lives for)
    int ttl = 30;
    float inertia = 0.05f;
    // Maximum sediment capactiy droplet can carry
    float sediment_cap = 4;
    float min_slope = 0.01f;
    // Speed of depositing sediment
    float deposit = 0.3f;
    // Speed of eroding
    float erode = 0.3f;
    // Speed of droplet evaporating
    float evaporate = 0.01f;
    float gravity = 4.0f;
};

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
};

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
            _map[id] = val * _amp;
        });
    }

    void hydro_erosion(ErosionConf conf = ErosionConf{});

    std::vector<Vertex> vertices() {
        std::vector<Vertex> vertices;
        for_each([&](int x, int y, int id) {
            vertices.push_back(
                { glm::vec3(x, _map[id], y), calc_normal(x, y) }
            );
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

    struct Droplet {
        float x = 0;
        float y = 0;
        float dir_x = 0;
        float dir_y = 0;
        float speed = 1;
        float water = 1;
        float sediment = 0;
    };

    float get_cell(int x, int y) const { return _map[x + y * _height]; }
    void set_cell(int x, int y, float val) { _map[x + y * _height] = val; }

    glm::vec3 calc_normal(int x, int y) const {
        int xl = std::max(x - 1, 0);
        int xr = std::min(x + 1, _width - 1);
        int yd = std::max(y - 1, 0);
        int yu = std::min(y + 1, _height - 1);

        float hl = _map[y * _width + xl];
        float hr = _map[y * _width + xr];
        float hd = _map[yd * _width + x];
        float hu = _map[yu * _width + x];

        glm::vec3 dx = glm::vec3(2.0f, hr - hl, 0.0f);
        glm::vec3 dy = glm::vec3(0.0f, hu - hd, 2.0f);
        return glm::normalize(glm::cross(dy, dx));
    }

    void sim_droplet(ErosionConf &conf, Droplet droplet);

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
