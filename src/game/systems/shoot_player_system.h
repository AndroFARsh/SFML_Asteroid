//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_SHOOT_PLAYER_SYSTEM_H
#define ECS_SHOOT_PLAYER_SYSTEM_H

#include <utility>

#include "../components/components.h"

#include "../../data/color.h"
#include "../../data/vector2.h"
#include "../../ecs/systems.h"
#include "../../utils/utils.h"

#include "../config/config.h"

class ShootPlayerSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    std::shared_ptr<Config> _config;

    std::shared_ptr<ecs::Filter> _filter = nullptr;

    std::shared_ptr<ecs::Pool<CTransform>> _transformPool = nullptr;
    std::shared_ptr<ecs::Pool<CInput>> _inputPool = nullptr;
    std::shared_ptr<ecs::Pool<CCooldown>> _cooldownPool = nullptr;

    std::shared_ptr<ecs::Pool<CProjectileTag>> _projectileTagPool = nullptr;
    std::shared_ptr<ecs::Pool<CVelocity>> _velocityPool = nullptr;
    std::shared_ptr<ecs::Pool<CShape>> _shapePool;
    std::shared_ptr<ecs::Pool<CLifespan>> _lifespanPool;
    std::shared_ptr<ecs::Pool<CCollider>> _colliderPool;

public:
    ShootPlayerSystem(std::shared_ptr<Config> config)
    : _config(std::move(config))
    {
    }

    void init(ecs::World& world) override {
        _transformPool = world.pool<CTransform>();
        _inputPool = world.pool<CInput>();
        _cooldownPool = world.pool<CCooldown>();

        _projectileTagPool = world.pool<CProjectileTag>();
        _velocityPool = world.pool<CVelocity>();
        _shapePool = world.pool<CShape>();
        _lifespanPool = world.pool<CLifespan>();
        _colliderPool = world.pool<CCollider>();

        _filter = world.buildFilter()
                .include<CPlayerTag>()
                .include<CTransform>()
                .include<CInput>()
                .exclude<CCooldown>()
                .build();
    }

    void run(ecs::World& world) override {
        for (auto entity : _filter->entities()) {
            const auto & input = _inputPool->get(entity);
            const auto & transform = _transformPool->get(entity);

            if (input.shoot) {
                spawnProjectile(world, transform.position, transform.forward());

                _cooldownPool->add(entity, _config->player.shootCooldown);
            }
        }
    }

    void spawnProjectile(ecs::World& world, Vector2 position, Vector2 forward) {
        auto entity = world.newEntity();

        _projectileTagPool->add(entity);
        _transformPool->add(entity, CTransform(position + forward * _config->player.radius));
        _shapePool->add(entity, createCShape(_config->projectile.radius, position, _config->projectile.fillColor));
        _velocityPool->add(entity, { forward * _config->projectile.speed });
        _colliderPool->add(entity, { _config->projectile.radius });
        _lifespanPool->add(entity, {_config->projectile.lifespan });
    }

    static CShape createCShape(float radius, Vector2 position, const Color& fillColor) {
        std::shared_ptr<sf::CircleShape> shape = std::make_shared<sf::CircleShape>(radius, 8);
        shape->setPosition(position());
        shape->setFillColor(fillColor());
        shape->setOrigin(0.5f, 0.5f);
        return { shape };
    }
};

#endif //ECS_SHOOT_PLAYER_SYSTEM_H