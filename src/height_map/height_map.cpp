#include "height_map.hpp"

#include <algorithm>
#include <cmath>
#include <random>

namespace tgl::height_map {

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

void HeightMap::sim_droplet(ErosionConf &conf, Droplet drop) {
    for (int ttl = conf.ttl; ttl >= 0 && drop.water >= 0.01f; --ttl) {
        int x = (int)drop.x;
        int y = (int)drop.y;
        if (x < 0 || x >= _width - 1 || y < 0 || y >= _height - 1)
            break;

        float h00 = get_cell(x, y);
        float h10 = get_cell(x + 1, y);
        float h01 = get_cell(x, y + 1);
        float h11 = get_cell(x + 1, y + 1);

        float fx = drop.x - x;
        float fy = drop.y - y;
        float height = h00 * (1 - fx) * (1 - fy) + h10 * fx * (1 - fy) +
                       h01 * (1 - fx) * fy + h11 * fx * fy;

        float grad_x = (h10 - h00) * (1 - fy) + (h11 - h01) * fy;
        float grad_y = (h01 - h00) * (1 - fx) + (h11 - h10) * fx;

        drop.dir_x = drop.dir_x * conf.inertia - grad_x * (1 - conf.inertia);
        drop.dir_y = drop.dir_y * conf.inertia - grad_y * (1 - conf.inertia);

        float len =
            std::sqrt(drop.dir_x * drop.dir_x + drop.dir_y * drop.dir_y);
        if (len > 0) {
            drop.dir_x /= len;
            drop.dir_y /= len;
        }

        drop.x += drop.dir_x;
        drop.y += drop.dir_y;
        if (drop.x < 0 || drop.x >= _width - 1 || drop.y < 0 ||
            drop.y >= _height - 1)
            break;

        float new_height = get_cell((int)drop.x, (int)drop.y);
        float delta_height = new_height - height;

        float cap = std::max(
            -delta_height * drop.speed * drop.water * conf.sediment_cap, 0.0f
        );
        if (delta_height > 0) {
            float amount = drop.sediment;
            drop.sediment -= amount;
            set_cell((int)drop.x, (int)drop.y, new_height + amount);
        } else {
            float cap = std::max(
                -delta_height * drop.speed * drop.water * conf.sediment_cap,
                0.0f
            );
            float amount =
                std::min((cap - drop.sediment) * conf.erode, -delta_height);
            if (amount > 0) {
                drop.sediment += amount;
                set_cell((int)drop.x, (int)drop.y, new_height - amount);
            }
        }

        drop.speed = std::sqrt(
            std::max(
                0.0f, drop.speed * drop.speed + delta_height * conf.gravity
            )
        );
        drop.water *= (1 - conf.evaporate);
    }
}

} // namespace tgl::height_map
