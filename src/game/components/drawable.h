//
// Created by Anton Kukhlevskyi on 2024-01-31.
//

#ifndef ECS_DRAWABLE_H
#define ECS_DRAWABLE_H

#include "SFML/Graphics.hpp"

#include "../../data/vector2.h"
#include "../../data/color.h"

struct CDrawable
{
    std::shared_ptr<sf::Drawable> value;
};

#endif //ECS_DRAWABLE_H
