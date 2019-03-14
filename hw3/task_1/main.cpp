#include <iostream>
#include <limits>
#include <cmath>

const double FLOAT_PRECISION = 1e-10;

struct Vector3D {
    double x, y, z;
};

std::istream &operator>>(std::istream &is, Vector3D &vec) {
    is >> vec.x >> vec.y >> vec.z;
    return is;
}

Vector3D lerp(const Vector3D &a, const Vector3D &b, double bias) {
    Vector3D result;
    result.x = (1 - bias) * a.x + bias * b.x;
    result.y = (1 - bias) * a.y + bias * b.y;
    result.z = (1 - bias) * a.z + bias * b.z;
    return result;
}

double vectorDistance(const Vector3D &a, const Vector3D &b) {
    return sqrt((a.x - b.x) * (a.x - b.x)
           + (a.y - b.y) * (a.y - b.y)
           + (a.z - b.z) * (a.z - b.z));
}

double segmentDistance(Vector3D a1, Vector3D a2, const Vector3D& b1, const Vector3D& b2) {
    double min_dist_prev = std::numeric_limits<double>::max();
    while (true) {
        Vector3D a_first = lerp(a1, a2, 1. / 4);
        Vector3D a_second = lerp(a1, a2, 3. / 4);

        double min_dist_first;
        double min_dist_second;
        double min_dist;

        if (b1.x == b2.x && b1.y == b2.y && b1.z == b2.z) {             //specialization to reduce copy-pasting
            min_dist_first = vectorDistance(a_first, b1);
            min_dist_second = vectorDistance(a_second, b1);
        } else {
            min_dist_first = segmentDistance(b1, b2, a_first, a_first);
            min_dist_second = segmentDistance(b1, b2, a_second, a_second);
        }

        if (min_dist_first < min_dist_second) {
            min_dist = min_dist_first;
            a2 = a_second;
        } else {
            min_dist = min_dist_second;
            a1 = a_first;
        }

        if (fabs(min_dist - min_dist_prev) < FLOAT_PRECISION) {
            return min_dist;
        }

        min_dist_prev = min_dist;
    }
}

int main() {
    Vector3D a1, a2, b1, b2;
    std::cin >> a1 >> a2 >> b1 >> b2;
    std::cout.setf(std::ostream::fixed, std::ostream::floatfield);
    std::cout.precision(7);
    std::cout << segmentDistance(a1, a2, b1, b2);
    return 0;
}
