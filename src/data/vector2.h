//
// Created by Anton Kukhlevskyi on 2024-01-31.
//

#ifndef INTROECS_VECTOR2_H
#define INTROECS_VECTOR2_H

#include "SFML/Graphics.hpp"
#include <iostream>

#define PI 3.14159265f

template <typename T>
struct Vector2T {

    T x = 0;
    T y = 0;

    Vector2T() = default;

    Vector2T(T x, T y)
            : x(x)
            , y(y)
    {
    }

    void print() const
    {
        std::cout << "Vector2T<"<< typeid(T).name() <<"> (x:" << x << ", y:" << y << ")" << std::endl;
    }

    Vector2T<T> operator + (const Vector2T<T> rv) const { return Vector2T<T>(x + rv.x, y + rv.y); }

    Vector2T<T> operator - (const Vector2T<T> rv) const { return Vector2T<T>(x - rv.x, y - rv.y); }

    Vector2T<T> operator - () { return Vector2T<T>(-x, -y); }

    void operator += (const Vector2T<T> rv)
    {
        x += rv.x;
        y += rv.y;
    }

    void operator -= (const Vector2T<T> rv)
    {
        x -= rv.x;
        y -= rv.y;
    }

    sf::Vector2<T> operator ()() const {
        return sf::Vector2<T>(x, y);
    }

    T sqrMagnitude() const { return x*x + y*y; }

    T magnitude() const { return std::sqrt(sqrMagnitude()); }

    Vector2T normalized() const {
        auto l = magnitude();
        return Vector2T(l > 0 ? x / l : 0, l > 0 ? y / l : 0);
    }

    Vector2T bounce(const Vector2T &p_normal) const {
        return -reflect(p_normal);
    }

    Vector2T reflect(const Vector2T &normal) const {
        return 2.0f * dot(normal) * normal - *this;
    }

    float dot(const Vector2T &v) const
    {
        return (x * v.x + y * v.y);
    }

    float angle(const Vector2T & to) const
    {
        auto num = std::sqrt(sqrMagnitude() * to.sqrMagnitude());
        return num < 1.0000000036274937E-15 ? 0.0f : std::acos(std::clamp(dot(to) / num, -1.f, 1.f)) * 57.29578f;
    }

    float signedAngle(const Vector2T &to) const {
        return angle(to) * ((((x * to.y - y *to.x)) >= 0.0) ? 1.f : -1.f);
    }

    Vector2T rotate(float angle) const {
        auto rad = angle * PI / 180.0f;
        return { x * std::cos(rad) - y * std::sin(rad), x * std::sin(rad) + y * std::cos(rad) };
    }
};

template <typename T>
Vector2T<T> operator * (const T & s, Vector2T<T> v) { return Vector2T<T>(s * v.x, s * v.y); }

template <typename T>
Vector2T<T> operator * (Vector2T<T> v, const T & s) { return operator*(s, v); }

typedef Vector2T<float> Vector2;
typedef Vector2T<int> Vector2Int;

#endif //INTROECS_VECTOR2_H
