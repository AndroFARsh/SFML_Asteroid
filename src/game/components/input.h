//
// Created by Anton Kukhlevskyi on 2024-01-31.
//

#ifndef ECS_INPUT_H
#define ECS_INPUT_H

#include <iostream>
#include <format>

struct CInput
{
    bool left = false;
    bool up = false;
    bool right = false;
    bool down = false;
    bool shoot = false;

    void print() const {
        std::cout
                << "left: " << left << " "
                << "up: " << up << " "
                << "right: " << right << " "
                << "down: " << down << " "
                << "shoot: " << shoot
                << std::endl;
    }
};

#endif //ECS_INPUT_H
