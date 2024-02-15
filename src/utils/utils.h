//
// Created by Anton Kukhlevskyi on 2024-02-08.
//

#ifndef INTROECS_UTILS_H
#define INTROECS_UTILS_H

#include <cstdlib>

constexpr static const int BIG_ENOUGH_INT = 16 * 1024;
constexpr static const double BIG_ENOUGH_FLOOR = BIG_ENOUGH_INT + 0.0000;

inline int fastFloor(float f) { return (int)(f + BIG_ENOUGH_FLOOR) - BIG_ENOUGH_INT; }

uint random(uint min, uint max) {
    auto rnd = std::rand();
    return min + rnd % (max - min);
}

int random(int min, int max) {
    auto rnd = std::rand();
    return min + rnd % (max - min);
}

float random(float min, float max) {
    float rnd = random((int)(min * 10000), (int)(max * 10000)) / 10000.0f;
    return rnd;
}


#endif //INTROECS_UTILS_H
