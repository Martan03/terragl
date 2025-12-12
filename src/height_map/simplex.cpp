#include "simplex.hpp"

#include <cmath>

namespace tgl::height_map {

Simplex::Simplex() : Noise() { }

float Simplex::noise(float x, float y) {
    auto s = (x + y) * _f2;
    auto xs = x + s;
    auto ys = y + s;
    auto i = std::floor(xs);
    auto j = std::floor(ys);

    auto t = (i + j) * _g2;
    auto x0 = x - (i - t);
    auto y0 = y - (j - t);

    int i1 = 0, j1 = 1;
    if (x0 > y0) {
        i1 = 1;
        j1 = 0;
    }

    auto x1 = x0 - i1 + _g2;
    auto y1 = y0 - j1 + _g2;

    auto x2 = x0 - 1 + 2 * _g2;
    auto y2 = y0 - 1 + 2 * _g2;

    auto gi0 = hash(i + hash(j));
    auto gi1 = hash(i + i1 + hash(j + j1));
    auto gi2 = hash(i + 1 + hash(j + 1));

    float n0 = 0, n1 = 0, n2 = 0;

    float t0 = 0.5 - x0 * x0 - y0 * y0;
    if (t0 >= 0) {
        t0 *= t0;
        n0 = t0 * t0 * grad(gi0, x0, y0);
    }

    float t1 = 0.5 - x1 * x1 - y1 * y1;
    if (t1 >= 0) {
        t1 *= t1;
        n1 = t1 * t1 * grad(gi1, x1, y1);
    }

    float t2 = 0.5 - x2 * x2 - y2 * y2;
    if (t2 >= 0) {
        t2 *= t2;
        n2 = t2 * t2 * grad(gi2, x2, y2);
    }

    return 45.23065f * (n0 + n1 + n2);
}

glm::vec3 Simplex::deriv_noise(float x, float y) {
    auto s = (x + y) * _f2;
    auto xs = x + s;
    auto ys = y + s;
    auto i = int(std::floor(xs));
    auto j = int(std::floor(ys));

    auto t = float(i + j) * _g2;
    auto x0 = x - (i - t);
    auto y0 = y - (j - t);

    int i1 = 0, j1 = 1;
    if (x0 > y0) {
        i1 = 1;
        j1 = 0;
    }

    auto x1 = x0 - i1 + _g2;
    auto y1 = y0 - j1 + _g2;

    auto x2 = x0 - 1 + 2 * _g2;
    auto y2 = y0 - 1 + 2 * _g2;

    int ii = i & 255, jj = j & 255;
    glm::vec3 res(0);

    int gi0 = _perm[ii + _perm[jj]];
    int gi1 = _perm[ii + i1 + _perm[jj + j1]];
    int gi2 = _perm[ii + 1 + _perm[jj + 1]];

    res += proc_corner(x0, y0, gi0);
    res += proc_corner(x1, y1, gi1);
    res += proc_corner(x2, y2, gi2);

    return 45.23065f * res;
}

uint8_t Simplex::hash(int i) {
    return _perm[i & 255];
}

float Simplex::grad(int hash, float x, float y) {
    auto h = hash & 7;
    auto u = h < 4 ? x : y;
    auto v = h < 4 ? y : x;
    return ((h & 1) ? -u : u) + ((h & 2) ? -2 * v : 2 * v);
}

glm::vec3 Simplex::proc_corner(float x, float y, int hash) {
    float t = 0.5 - x * x - y * y;
    if (t < 0) {
        return glm::vec3(0);
    }
    float t3 = t * t * t;
    float t4 = t3 * t;

    auto g = GRAD3[hash & 7];
    float gx = g[0], gy = g[1];
    float p = gx * x + gy * y;

    float k = 8 * t3 * p;
    float nx = t4 * gx - k * x;
    float ny = t4 * gy - k * y;
    return glm::vec3(nx, ny, t4 * p);
}

} // namespace tgl::height_map
