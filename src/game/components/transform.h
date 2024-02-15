//
// Created by Anton Kukhlevskyi on 2024-01-31.
//

#ifndef ECS_TRANSFORM_H
#define ECS_TRANSFORM_H

#include <iostream>
#include "../../data/vector2.h"

struct CTransform
{
    Vector2 position = {0, 0};
    float rotation = 0;
    float scale = 1;

    Vector2 forward() const { return Vector2(0, -1).rotate(rotation); }

    explicit CTransform(const Vector2& position, float rotation = 0, float scale = 1)
    : position(position)
    , rotation(rotation)
    , scale(scale)
    {
    }

    void print() const {
        std::cout
                << "position: (" << position.x << ", " << position.y << ") "
                << "rotation: " << rotation << " "
                << "scale: " << scale
                << std::endl;
    }
};

#endif //ECS_TRANSFORM_H
