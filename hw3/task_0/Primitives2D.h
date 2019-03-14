#pragma once

class Point2D {
public:
    typedef double componentType;

protected:
    componentType x_, y_;

public:
    explicit Point2D(componentType x = 0, componentType y = 0);
    bool operator==(const Point2D& other) const;
    const componentType& x() const;
    const componentType& x(componentType x);
    const componentType& y() const;
    const componentType& y(componentType y);
};

class Vector2D : public Point2D {
public:
    explicit Vector2D(componentType x = 0, componentType y = 0);
    Vector2D(Point2D a, Point2D b);        //A - B vector
    explicit Vector2D(Point2D);
    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);
    Vector2D& operator*=(const componentType& scalar);
    Vector2D& operator/=(const componentType& scalar);
    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator-() const;
    componentType dot(const Vector2D &other) const;
    componentType cross_z(const Vector2D &other) const;
};

Vector2D operator*(Vector2D vec, const Point2D::componentType& scalar);
Vector2D operator*(const Point2D::componentType& scalar, Vector2D vec);
Vector2D operator/(Vector2D vec, const Point2D::componentType& scalar);