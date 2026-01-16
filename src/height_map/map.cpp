#include "map.hpp"

#include <algorithm>
#include <cmath>
#include <queue>
#include <random>
#include <unordered_map>

#include "perlin.hpp"
#include "simplex.hpp"

namespace tgl::height_map {

Map::Map(
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
    _heights(width * height),
    _normals(width * height),
    _water(width * height, -_amp),
    _freq(freq),
    _amp(amp),
    _lacunarity(lacunarity),
    _persist(persist) { }

void Map::gen(NoiseType type, int oct) {
    switch (type) {
    case NoiseType::Perlin:
        gen_perlin(oct);
        break;
    case NoiseType::Simplex:
        gen_simplex(oct);
        break;
    }
    normals_gen();
}

void Map::gen_perlin(int oct) {
    auto perlin = Perlin();
    noise_gen(perlin, oct);
}

void Map::gen_simplex(int oct) {
    auto simplex = Simplex();
    noise_gen(simplex, oct);
}

void Map::hydro_erosion(ErosionConf conf) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(0, 1);

    std::unordered_map<int, float> droplets;
    for (int i = 0; i < conf.droplets; ++i) {
        float x = dist(gen) * (_width - 1);
        float y = dist(gen) * (_height - 1);
        auto droplet = Droplet{ x, y };
        auto pos = sim_droplet(conf, droplet);
        auto id = (int)pos.y * _width + (int)pos.x;
        droplets[id] += 1.0;
    }

    // for (auto const &[index, count] : droplets) {
    //     if (index < 0 || index >= _water.size() || count < conf.lake_tresh)
    //         continue;
    //     int x = index % _width, y = index / _width;
    //     // if (!is_local_min(x, y))
    //     //     continue;
    //     float level = _heights[index] + (count * conf.depth_scale);
    //     level = std::min(find_spill(x, y), level);
    //     process_lake(x, y, level);
    // }

    for (auto const &[id, count] : droplets) {
        if (id < 0 || id > _water.size() || count < conf.lake_tresh)
            continue;

        float level = _heights[id] + count * conf.depth_scale;
        process_lake(id, level);
    }

    // for (auto const &[id, count] : droplets) {
    //     if (id < 0 || id >= _water.size() || count < conf.lake_tresh)
    //         continue;
    //     float level = _heights[id] + count * conf.depth_scale;
    //     _water[id] = level;
    //     if (id - 1 >= 0)
    //         _water[id - 1] = level;
    //     if (id + 1 < _water.size())
    //         _water[id + 1] = level;
    //     if (id + _width < _water.size())
    //         _water[id + _width] = level;
    //     if (id + _width - 1 < _water.size())
    //         _water[id + _width - 1] = level;
    //     if (id + _width + 1 < _water.size())
    //         _water[id + _width + 1] = level;
    //     if (id - _width >= 0)
    //         _water[id - _width] = level;
    //     if (id - _width - 1 >= 0)
    //         _water[id - _width - 1] = level;
    //     if (id - _width + 1 >= 0)
    //         _water[id - _width + 1] = level;
    // }

    // auto max = 0.0001f;
    // for (int id = 0; id < _width * _height; ++id) {
    //     max = std::max(max, _water[id]);
    // }

    // for (int id = 0; id < _width * _height; ++id) {
    //     _water[id] /= max;
    // }
}

std::vector<Vertex> Map::vertices(int factor) {
    std::vector<Vertex> vertices;
    for (int y = 0; y < _height; y += factor) {
        for (int x = 0; x < _width; x += factor) {
            auto id = y * _width + x;
            auto rx = float(x) * _x_rate;
            auto ry = float(y) * _y_rate;
            vertices.push_back(
                { glm::vec3(rx, _heights[id], ry),
                  glm::vec2(float(x) / _width, float(y) / _height) }
            );
        }
    }
    return vertices;
}

std::vector<unsigned> Map::indices(int factor) {
    std::vector<unsigned int> indices;
    auto w = (_width + factor - 1) / factor;
    auto h = (_height + factor - 1) / factor;
    for (int y = 0; y < h - 1; ++y) {
        for (int x = 0; x < w - 1; ++x) {
            int id = y * w + x;

            indices.push_back(id);
            indices.push_back(id + w);
            indices.push_back(id + w + 1);
            indices.push_back(id + 1);
        }
    }
    return indices;
}

std::vector<unsigned char> Map::pixels() {
    auto cnt = _width * _height;
    std::vector<unsigned char> pixels(cnt);
    auto r = 0.5f / _amp;
    for (int i = 0; i < cnt; ++i) {
        pixels[i] =
            static_cast<unsigned char>((_heights[i] * r + 0.5f) * 255.0f);
    }
    return pixels;
}

void Map::noise_gen(Noise &noise, int oct) {
    for_each([&](int x, int y, int id) {
        auto rx = (float)x * _x_rate * _freq;
        auto ry = (float)y * _y_rate * _freq;
        // float val = noise.fbm(rx, ry, oct);
        // float val = noise.fbm(rx, ry, 0.5f, oct);
        // auto val = noise.deriv_fbm(rx, ry, oct);
        float val = noise.ridged_fbm(rx, ry, 0.5f, oct);
        // _heights[id] = val.z * _amp;
        _heights[id] = (std::pow(val * 0.5 + 0.5, 3) * 2 - 1) * _amp;
    });
}

void Map::normals_gen() {
    for_each([&](int x, int y, int id) {
        float hl = _heights[y * _width + std::max(x - 1, 0)];
        float hr = _heights[y * _width + std::min(x + 1, _width - 1)];
        float hd = _heights[std::max(y - 1, 0) * _width + x];
        float hu = _heights[std::min(y + 1, _height - 1) * _width + x];

        glm::vec3 dx(2.0f, hr - hl, 0.0f);
        glm::vec3 dy(0.0f, hu - hd, 2.0f);

        auto norm = glm::normalize(glm::cross(dy, dx));
        _normals[id] = norm;
    });
}

glm::vec2 Map::sim_droplet(ErosionConf &conf, Droplet drop) {
    for (int ttl = conf.ttl; ttl >= 0 && drop.water >= 0.01f; --ttl) {
        auto x = (int)drop.x, y = (int)drop.y;
        if (x < 0 || x >= _width - 1 || y < 0 || y >= _height - 1)
            return glm::vec2(-1, -1);

        auto id = x + y * _width;
        auto fx = drop.x - x, fy = drop.y - y;

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

            _heights[id] += amount * (1 - fx) * (1 - fy);
            _heights[id + 1] += amount * fx * (1 - fy);
            _heights[id + _width] += amount * (1 - fx) * fy;
            _heights[id + _width + 1] += amount * fx * fy;
        } else {
            auto amount =
                std::min((cap - drop.sediment) * conf.erode, -delta_height);
            drop.sediment += amount;

            _heights[id] -= amount * (1 - fx) * (1 - fy);
            _heights[id + 1] -= amount * fx * (1 - fy);
            _heights[id + _width] -= amount * (1 - fx) * fy;
            _heights[id + _width + 1] -= amount * fx * fy;
        }

        drop.speed = std::sqrt(
            std::max(
                0.0f, drop.speed * drop.speed + delta_height * conf.gravity
            )
        );
        drop.water *= (1 - conf.evaporate);
    }
    return glm::vec2(drop.x, drop.y);
}

std::tuple<float, float, float> Map::calc(Droplet drop) {
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

bool Map::is_local_min(int x, int y) {
    int dx[] = { 0, 0, 1, -1 };
    int dy[] = { 1, -1, 0, 0 };

    auto height = _heights[y * _width + x];
    for (int i = 0; i < 4; ++i) {
        auto nx = x + dx[i], ny = y + dy[i];
        if (nx < 0 || nx >= _width || ny < 0 || ny >= _height) {
            continue;
        }

        auto idx = ny * _width + nx;
        if (_heights[idx] < height)
            return false;
    }
    return true;
}

float Map::find_spill(int x, int y) {
    std::vector<bool> closed(_width * _height, false);
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open;

    auto id = y * _width + x;
    float max_height = _heights[id];
    open.push({ x, y, max_height });
    closed[id] = true;

    while (!open.empty()) {
        auto cur = open.top();
        open.pop();

        max_height = std::max(max_height, cur.h);
        if (cur.x == 0 || cur.x == _width - 1 || cur.y == 0 ||
            cur.y == _height - 1) {
            return max_height;
        }

        int dx[] = { 0, 0, 1, -1 };
        int dy[] = { 1, -1, 0, 0 };
        for (int i = 0; i < 4; ++i) {
            auto nx = cur.x + dx[i], ny = cur.y + dy[i];
            if (nx < 0 || nx >= _width || ny < 0 || ny >= _height) {
                continue;
            }

            auto idx = ny * _width + nx;
            if (closed[idx])
                continue;

            closed[idx] = true;
            if (_heights[idx] < max_height) {
                return max_height;
            }
            open.push({ nx, ny, _heights[idx] });
        }
    }
    return max_height;
}

void Map::process_lake(int id, float level) {
    if (_water[id] <= -_amp + 0.01)
        return;

    std::queue<int> open;
    open.push(id);
    _water[id] = level;

    while (!open.empty()) {
        id = open.front();
        open.pop();

        int cx = id % _width, cy = id / _width;
        int dx[] = { -1, 1, 0, 0 };
        int dy[] = { 0, 0, -1, 1 };
        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i], ny = cy + dy[i];
            if (nx < 0 || nx >= _width || ny < 0 || ny >= _height)
                continue;

            int idx = ny * _width + nx;
            if (_heights[idx] < level && _water[idx] <= -_amp + 0.01) {
                _water[idx] = level;
                open.push(idx);
            }
        }
    }
}

} // namespace tgl::height_map
