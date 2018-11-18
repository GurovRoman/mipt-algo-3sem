#include <iostream>
#include "Primitives2D.h"

int main() {
    Point2D a(1.3f, 10), b(5, 9);
    Vector2D vec1(a, b), vec2(b, a);
    Vector2D vec3(-vec1.y(), vec1.x());
    std::cout << vec1.dot(vec2) << ' ' << vec1.cross_z(vec2) << ' '
              << vec1.dot(vec3) << ' ' << vec1.cross_z(vec3) << ' '
              << vec2.dot(vec3) << ' ' << vec2.cross_z(vec3);
}
