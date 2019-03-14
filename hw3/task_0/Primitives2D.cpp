#include <utility>
#include "Primitives2D.h"

Point2D::Point2D(componentType x, componentType y) : x_(std::move(x)),
                                                     y_(std::move(y)) {}

bool Point2D::operator==(const Point2D &other) const {
    return x_ == other.x_ && y_ == other.y_;
}

const Point2D::componentType &Point2D::x() const {
    return x_;
}

const Point2D::componentType &Point2D::x(Point2D::componentType x) {
    x_ = std::move(x);
    return x_;
}

const Point2D::componentType &Point2D::y() const {
    return y_;
}

const Point2D::componentType &Point2D::y(Point2D::componentType y) {
    y_ = std::move(y);
    return y_;
}


Vector2D::Vector2D(Point2D::componentType x,
                   Point2D::componentType y) : Point2D(std::move(x),
                                                       std::move(y)) {}

Vector2D::Vector2D(Point2D a, Point2D b) : Point2D(std::move(a)){
    *this -= Vector2D(std::move(b));
}

Vector2D::Vector2D(Point2D other) : Point2D(std::move(other)) {}

Vector2D &Vector2D::operator+=(const Vector2D &other) {
    x_ += other.x_;
    y_ += other.y_;
    return *this;
}

Vector2D &Vector2D::operator-=(const Vector2D &other) {
    x_ -= other.x_;
    y_ -= other.y_;
    return *this;
}

Vector2D &Vector2D::operator*=(const Point2D::componentType &scalar) {
    x_ *= scalar;
    y_ *= scalar;
    return *this;
}

Vector2D &Vector2D::operator/=(const Point2D::componentType &scalar) {
    x_ /= scalar;
    y_ /= scalar;
    return *this;
}

Vector2D::componentType Vector2D::dot(const Vector2D &other) const {
    return x_ * other.x_ + y_ * other.y_;
}

Vector2D::componentType Vector2D::cross_z(const Vector2D &other) const {
    return x_ * other.y_ - y_ * other.x_;
}

Vector2D Vector2D::operator+(const Vector2D &other) const {
    auto result(*this);
    return result += other;
}

Vector2D Vector2D::operator-(const Vector2D &other) const {
    auto result(*this);
    return result -= other;
}

Vector2D Vector2D::operator-() const {
    return Vector2D() - *this;
}


Vector2D operator*(Vector2D vec, const Point2D::componentType &scalar) {
    return vec *= scalar;
}

Vector2D operator*(const Point2D::componentType &scalar, Vector2D vec) {
    return vec *= scalar;
}

Vector2D operator/(Vector2D vec, const Point2D::componentType &scalar) {
    return vec /= scalar;
}
