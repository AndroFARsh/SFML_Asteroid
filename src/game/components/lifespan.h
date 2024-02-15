//
// Created by Anton Kukhlevskyi on 2024-01-31.
//

#ifndef ECS_LIFESPAN_H
#define ECS_LIFESPAN_H

#include <iostream>

struct CLifespan
{
    const float total = 0;
    float current = 0;

    CLifespan(float total)
    : total(total)
    , current(total)
    {
    }

    void print() const {
        std::cout << "current:" << current << " total: " << total << std::endl;
    }
};

#endif //ECS_LIFESPAN_H
