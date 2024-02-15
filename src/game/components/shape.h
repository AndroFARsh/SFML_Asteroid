//
// Created by Anton Kukhlevskyi on 2024-01-31.
//

#ifndef ECS_SHAPE_H
#define ECS_SHAPE_H

#include "SFML/Graphics.hpp"

#include "../../data/vector2.h"
#include "../../data/color.h"

struct CShape
{
    std::shared_ptr<sf::Shape> value = nullptr;
};

#endif //ECS_SHAPE_H
