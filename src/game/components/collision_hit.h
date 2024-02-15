//
// Created by Anton Kukhlevskyi on 2024-01-31.
//

#ifndef ECS_COLLISION_HIT_H
#define ECS_COLLISION_HIT_H

#include "../../data/vector2.h"

enum ObjectType {
    Player,
    Asteroid,
    Fragment,
    Projectile
};

struct CCollisionHit {
    Vector2 velocity;
    Vector2 position;
    float radius;
    float distance;
    ObjectType type;
};

#endif //ECS_COLLISION_HIT_H
