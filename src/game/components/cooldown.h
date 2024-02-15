//
// Created by Anton Kukhlevskyi on 2024-01-31.
//

#ifndef ECS_COOLDOWN_H
#define ECS_COOLDOWN_H

#include <iostream>

struct CCooldown
{
    const float total = 0;
    float current = 0;

    CCooldown(float total)
    : total(total)
    , current(total)
    {
    }
};

#endif //ECS_COOLDOWN_H
