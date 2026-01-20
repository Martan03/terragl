#pragma once

#include <tuple>
#include <vector>

#include "noise.hpp"

#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>

namespace tgl::height_map {

struct ErosionConf {
    // Number of droplets to simulate
    int droplets = 70000;
    // TTL of droplet (steps it lives for)
    int ttl = 90;
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
    // Treshold for the amount of water to create a lake
    int lake_tresh = 10;
    // Depth scale per droplet in the lake
    float depth_scale = 0.00015f;
};

struct Vertex {
    glm::vec3 pos;
    glm::vec2 uv;
};

class Map {
public:
    Map(int width,
        int height,
        float freq = 0.03,
        float amp = 35,
        float lacunarity = 2,
        float persist = 0.5);

    void gen(NoiseType noise, FbmType fbm, int oct = 5);
    void gen_perlin(FbmType fbm, int oct = 5);
    void gen_simplex(FbmType fbm, int oct = 5);

    void hydro_erosion(ErosionConf conf = ErosionConf{});

    std::vector<Vertex> vertices(int factor = 1);
    std::vector<unsigned int> indices(int factor = 1);
    std::vector<unsigned char> pixels();

    int width() { return _width; }
    int height() { return _height; }
    float amp() { return _amp; }

    std::vector<float> &heights() { return _heights; }
    std::vector<glm::vec3> &normals() { return _normals; }
    std::vector<float> &water() { return _water; }

private:
    int _width, _height;
    float _x_rate, _y_rate;
    float _freq = 0.05, _amp = 20;
    float _lacunarity = 2, _persist = 0.5;

    std::vector<float> _heights;
    std::vector<glm::vec3> _normals;
    std::vector<float> _water;

    void noise_gen(FbmType fbm, Noise &noise, int oct = 1);
    void normals_gen();

    struct Droplet {
        float x = 0, y = 0;
        float dir_x = 0, dir_y = 0;
        float speed = 1;
        float water = 1;
        float sediment = 0;
    };

    struct Node {
        int x, y;
        float h;

        bool operator>(const Node &other) const { return h > other.h; }
        bool operator<(const Node &other) const { return h < other.h; }
    };

    float get_cell(int x, int y) const { return _heights[x + y * _width]; }
    void set_cell(int x, int y, float val) { _heights[x + y * _width] = val; }

    glm::vec3 calc_normal(int x, int y) const {
        int xl = std::max(x - 1, 0);
        int xr = std::min(x + 1, _width - 1);
        int yd = std::max(y - 1, 0);
        int yu = std::min(y + 1, _height - 1);

        float hl = _heights[y * _width + xl];
        float hr = _heights[y * _width + xr];
        float hd = _heights[yd * _width + x];
        float hu = _heights[yu * _width + x];

        glm::vec3 dx = glm::vec3(2.0f, hr - hl, 0.0f);
        glm::vec3 dy = glm::vec3(0.0f, hu - hd, 2.0f);
        return glm::normalize(glm::cross(dy, dx));
    }

    glm::vec2 sim_droplet(ErosionConf &conf, Droplet &droplet);
    std::tuple<float, float, float> calc(Droplet droplet);

    int find_sink(int x, int y);
    float find_spill(int x, int y);
    void process_lake(int id, float level);

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
