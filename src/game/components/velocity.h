//
// Created by Anton Kukhlevskyi on 2024-01-31.
//

#ifndef ECS_VELOCITY_H
#define ECS_VELOCITY_H

#include <iostream>
#include "../../data/vector2.h"

struct CRotationVelocity {
    float value;
};

struct CVelocity
{
    Vector2 value = {0, 0};

    CVelocity() = default;

    CVelocity(const Vector2 v)
    : value(v)
    {
    }

    CVelocity(float x, float y)
    : value(Vector2(x, y))
    {
    }

    void print() const {
        std::cout
            << "velocity: (" << value.x << ", " << value.y << ")"
            << std::endl;
    }
};

#endif //ECS_VELOCITY_H
