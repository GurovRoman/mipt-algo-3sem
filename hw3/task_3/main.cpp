#include <iostream>
#include <vector>
#include <cmath>
#include "../task_0/Primitives2D.h"

using std::vector;

double eps = 1e-32;

bool operator<(Vector2D a, Vector2D b) {
    if (a.y() > eps && b.y() < -eps)
        return true;

    if (a.y() < -eps && b.y() > eps)
        return false;

    return a.cross_z(b) > -eps;
}

vector<Vector2D> minkowski_sum(const vector<Vector2D> &first, const vector<Vector2D> &second) {
    vector<Vector2D> result;

    size_t first_offset = 0;
    for (size_t i = 1; i < first.size(); ++i) {
        auto cur = first[i];
        auto prev = first[first_offset];
        if (cur.x() < prev.x()) {
            first_offset = i;
        }
        if (cur.x() == prev.x() && cur.y() < prev.y()) {
            first_offset = i;
        }
    }
    size_t second_offset = 0;
    for (size_t i = 1; i < second.size(); ++i) {
        auto cur = second[i];
        auto prev = second[second_offset];
        if (cur.x() < prev.x()) {
            second_offset = i;
        }
        if (cur.x() == prev.x() && cur.y() < prev.y()) {
            second_offset = i;
        }
    }

    size_t i = first_offset;
    size_t j = second_offset;
    while (i < first.size() + first_offset || j < second.size() + second_offset) {
        auto first_i = first[i % first.size()];
        auto first_i_1 = first[(i + 1) % first.size()];
        auto second_j = second[j % second.size()];
        auto second_j_1 = second[(j + 1) % second.size()];

        result.push_back(first_i + second_j);

        if (first_i_1 - first_i < second_j_1 - second_j) {
            ++j;
        }
        if (second_j_1 - second_j < first_i_1 - first_i) {
            ++i;
        }
    }
    return result;
}

bool zero_in_polygon(const vector<Vector2D> &polygon) {
    for (size_t i = 2; i < polygon.size(); ++i) {
        if ((polygon[i] - polygon[0] < -polygon[0]) && (-polygon[0] < polygon[i - 1] - polygon[0])) {
            return polygon[i + 1].cross_z(polygon[i]) > -eps;
        }
    }
    return false;
}

int main() {
    size_t n, m;
    vector<Vector2D> first, second;
    std::cin >> n;
    for (size_t i = 0; i < n; ++i) {
        double x, y;
        std::cin >> x >> y;
        first.emplace_back(x, y);
    }
    std::cin >> m;
    for (size_t i = 0; i < m; ++i) {
        double x, y;
        std::cin >> x >> y;
        second.emplace_back(-x, -y);
    }
    auto minkowski = minkowski_sum(first, second);
    for (auto a : minkowski) {
        std::cout << a.x() << ' ' << a.y() << '\n';
    }
    std::cout << (zero_in_polygon(minkowski) ? "YES" : "NO");
}
