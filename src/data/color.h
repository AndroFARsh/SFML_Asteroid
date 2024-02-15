//
// Created by Anton Kukhlevskyi on 2024-01-31.
//

#ifndef INTROECS_COLOR_H
#define INTROECS_COLOR_H

#include "SFML/Graphics.hpp"
#include <iostream>
#


struct Color {
    uint r; //!< Red component
    uint g; //!< Green component
    uint b; //!< Blue component
    uint a; //!< Alpha (opacity) component

    Color() = default;

    Color(uint r, uint g, uint b, uint a = 255)
    : r(r)
    , g(g)
    , b(b)
    , a(a)
    {
    }

    void print() const
    {
        std::cout << "Color (r:" << r << ", g:" << g << ", b:" << b << ", a:" << a << ")" << std::endl;
    }

    sf::Color operator ()() const {
        return {
            std::clamp<u_int8_t>(r, 0, 255),
            std::clamp<u_int8_t>(g, 0, 255),
            std::clamp<u_int8_t>(b, 0, 255),
            std::clamp<u_int8_t>(a, 0, 255)
        };
    }
};

#endif //INTROECS_COLOR_H
