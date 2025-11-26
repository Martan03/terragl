#include "height_map.hpp"

#include <algorithm>
#include <cmath>
#include <random>

#include "perlin.hpp"
#include "simplex.hpp"

namespace tgl::height_map {

HeightMap::HeightMap(
    int width,
    int height,
    float freq,
    float amp,
    float lacunarity,
    float persist
) :
    _width(width),
    _x_rate(256.0 / width),
    _height(height),
    _y_rate(256.0 / height),
    _map(width * height),
    _freq(freq),
    _amp(amp),
    _lacunarity(lacunarity),
    _persist(persist) { }

void HeightMap::gen(NoiseType type, int oct) {
    switch (type) {
    case NoiseType::Perlin:
        gen_perlin(oct);
        break;
    case NoiseType::Simplex:
        gen_simplex(oct);
        break;
    }
}

void HeightMap::gen_perlin(int oct) {
    auto perlin = Perlin();
    noise_gen(perlin, oct);
}

void HeightMap::gen_simplex(int oct) {
    auto simplex = Simplex();
    noise_gen(simplex, oct);
}

void HeightMap::hydro_erosion(ErosionConf conf) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0, 1);

    for (int i = 0; i < conf.droplets; ++i) {
        float x = dist(gen) * (_width - 1);
        float y = dist(gen) * (_height - 1);
        auto droplet = Droplet{ x, y };
        sim_droplet(conf, droplet);
    }
}

void HeightMap::noise_gen(Noise &noise, int oct) {
    for_each([&](int x, int y, int id) {
        auto rx = (float)x * _x_rate * _freq;
        auto ry = (float)y * _y_rate * _freq;
        // float val = noise.fbm(rx, ry, oct);
        // float val = noise.fbm(rx, ry, 0.5f, oct);
        float val = noise.ridged_fbm(rx, ry, oct);
        // _map[id] = val * _amp;
        _map[id] = (std::pow(val * 0.5 + 0.5, 3) * 2 - 1) * _amp;
    });
}

void HeightMap::sim_droplet(ErosionConf &conf, Droplet drop) {
    for (int ttl = conf.ttl; ttl >= 0 && drop.water >= 0.01f; --ttl) {
        auto x = (int)drop.x;
        auto y = (int)drop.y;
        if (x < 0 || x >= _width - 1 || y < 0 || y >= _height - 1)
            break;

        auto id = x + y * _width;
        auto fx = drop.x - x;
        auto fy = drop.y - y;

        auto [height, grad_x, grad_y] = calc(drop);
        drop.dir_x = drop.dir_x * conf.inertia - grad_x * (1 - conf.inertia);
        drop.dir_y = drop.dir_y * conf.inertia - grad_y * (1 - conf.inertia);

        auto len =
            std::sqrt(drop.dir_x * drop.dir_x + drop.dir_y * drop.dir_y);
        if (len != 0) {
            drop.dir_x /= len;
            drop.dir_y /= len;
        }

        drop.x += drop.dir_x;
        drop.y += drop.dir_y;
        if ((drop.dir_x == 0 && drop.dir_y == 0) || (int)drop.x < 0 ||
            (int)drop.x >= _width - 1 || (int)drop.y < 0 ||
            (int)drop.y >= _height - 1) {
            break;
        }

        auto [new_height, _gx, _gy] = calc(drop);
        auto delta_height = new_height - height;

        auto cap = std::max(
            -delta_height * drop.speed * drop.water * conf.sediment_cap, 0.01f
        );
        if (drop.sediment > cap || delta_height > 0) {
            auto amount = (delta_height > 0)
                              ? std::min(delta_height, drop.sediment)
                              : (drop.sediment - cap) * conf.deposit;
            drop.sediment -= amount;

            _map[id] += amount * (1 - fx) * (1 - fy);
            _map[id + 1] += amount * fx * (1 - fy);
            _map[id + _width] += amount * (1 - fx) * fy;
            _map[id + _width + 1] += amount * fx * fy;
        } else {
            auto amount =
                std::min((cap - drop.sediment) * conf.erode, -delta_height);
            drop.sediment += amount;

            _map[id] -= amount * (1 - fx) * (1 - fy);
            _map[id + 1] -= amount * fx * (1 - fy);
            _map[id + _width] -= amount * (1 - fx) * fy;
            _map[id + _width + 1] -= amount * fx * fy;
        }

        drop.speed = std::sqrt(
            std::max(
                0.0f, drop.speed * drop.speed + delta_height * conf.gravity
            )
        );
        drop.water *= (1 - conf.evaporate);
    }
}

std::tuple<float, float, float> HeightMap::calc(Droplet drop) {
    int x = (int)drop.x;
    int y = (int)drop.y;

    float h00 = get_cell(x, y);         // NW
    float h10 = get_cell(x + 1, y);     // NE
    float h01 = get_cell(x, y + 1);     // SW
    float h11 = get_cell(x + 1, y + 1); // SE

    float fx = drop.x - x;
    float fy = drop.y - y;
    float height = h00 * (1 - fx) * (1 - fy) + h10 * fx * (1 - fy) +
                   h01 * (1 - fx) * fy + h11 * fx * fy;

    float grad_x = (h10 - h00) * (1 - fy) + (h11 - h01) * fy;
    float grad_y = (h01 - h00) * (1 - fx) + (h11 - h10) * fx;

    return { height, grad_x, grad_y };
}

} // namespace tgl::height_map
