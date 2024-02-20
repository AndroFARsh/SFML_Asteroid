//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_COLLIDE_OBSTACLE_SYSTEM_H
#define ECS_COLLIDE_OBSTACLE_SYSTEM_H

#include "../components/components.h"

#include "../../data/color.h"
#include "../../data/vector2.h"
#include "../../ecs/systems.h"
#include "../../utils/utils.h"

class CollideSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    const std::string _name = "CollideSystem";

    std::shared_ptr<ecs::Filter> _filter;

    std::shared_ptr<ecs::Pool<CCollisionHit>> _collisionHitPool;
    std::shared_ptr<ecs::Pool<CCollider>> _colliderPool;
    std::shared_ptr<ecs::Pool<CTransform>> _transformPool;
    std::shared_ptr<ecs::Pool<CVelocity>> _velocityPool;

    std::shared_ptr<ecs::Pool<CPlayerTag>> _playerTagPool;
    std::shared_ptr<ecs::Pool<CAsteroidTag>> _asteroidTagPool;
    std::shared_ptr<ecs::Pool<CProjectileTag>> _projectileTagPool;

public:
    [[nodiscard]] const std::string& name() const override { return _name; }

    void init(ecs::World &world) override {
        _transformPool = world.pool<CTransform>();
        _velocityPool = world.pool<CVelocity>();
        _colliderPool = world.pool<CCollider>();
        _collisionHitPool = world.pool<CCollisionHit>();

        _playerTagPool = world.pool<CPlayerTag>();
        _asteroidTagPool = world.pool<CAsteroidTag>();
        _projectileTagPool = world.pool<CProjectileTag>();

        _filter = world.buildFilter()
                .include<CCollider>()
                .include<CTransform>()
                .include<CVelocity>()
                .build();
    }

    void run(ecs::World &world, const sf::Time& dt) override {
        for (auto entity: _filter->entities()) {
            checkCollision(entity);
        }
    }

    void checkCollision(const ecs::Entity &entity) {
        const auto &collider = _colliderPool->get(entity);
        const auto &transform = _transformPool->get(entity);

        // todo: iterate over nearest object
        for (auto otherEntity: _filter->entities()) {
            if (entity == otherEntity) continue;

            const auto &otherCollider = _colliderPool->get(otherEntity);
            const auto &otherTransform = _transformPool->get(otherEntity);
            const auto &otherVelocity = _velocityPool->get(otherEntity);

            auto actualDistance = (transform.position - otherTransform.position).magnitude();
            auto expectedDistance = collider.value + otherCollider.value;

            // check is object collided
            if (actualDistance < expectedDistance) {
                if (!_collisionHitPool->has(entity)) { _collisionHitPool->add(entity); }
                auto & hit = _collisionHitPool->get(entity);

                hit.velocity = otherVelocity.value;
                hit.position = otherTransform.position;
                hit.radius   = otherCollider.value;
                hit.type     = toType(otherEntity);
                hit.distance = actualDistance;

                return;
            }
        }
    }

    ObjectType toType(const ecs::Entity &entity) {
        if (_playerTagPool->has(entity)) return Player;
        if (_asteroidTagPool->has(entity)) return Asteroid;
        if (_projectileTagPool->has(entity)) return Projectile;

        return Fragment;
    }
};

#endif //ECS_COLLIDE_OBSTACLE_SYSTEM_H
