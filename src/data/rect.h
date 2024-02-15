//
// Created by Anton Kukhlevskyi on 2024-01-31.
//

#ifndef INTROECS_RECT_H
#define INTROECS_RECT_H

#include "vector2.h"

template <typename T>
struct RectT {
    T left = 0;
    T top = 0;
    T width = 0;
    T heigth = 0;

    RectT() = default;

    RectT(T l, T t, T width, T height)
    : left(l)
    , top(t)
    , width(t)
    , heigth(t)
    {
    }

    T right() { return left + width; }

    T bottom() { return top + heigth; }

    Vector2 size() { return { width , heigth }; }

    Vector2 halfSize() { return { width >> 1 , heigth >> 1 }; }

    Vector2 center() { return { left + (width >> 1), top + (heigth >> 1) }; }
};

typedef RectT<float> Rect;
typedef RectT<int> RectInt;

#endif //INTROECS_RECT_H
