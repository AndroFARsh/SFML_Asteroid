//
// Created by Anton Kukhlevskyi on 2024-01-31.
//

#ifndef ECS_TEXT_H
#define ECS_TEXT_H

#include "SFML/Graphics.hpp"

#include "../../data/vector2.h"
#include "../../data/color.h"

struct CText
{
    std::shared_ptr<sf::Text> value = nullptr;
};

#endif //ECS_TEXT_H
