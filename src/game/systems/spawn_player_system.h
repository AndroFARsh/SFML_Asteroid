//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_SPAWN_PLAYER_SYSTEM_H
#define ECS_SPAWN_PLAYER_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <utility>
#include "../components/components.h"

#include "../../data/color.h"
#include "../../data/vector2.h"

#include "../../ecs/systems.h"

class SpawnPlayerSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    std::shared_ptr<Config> _config;

    std::shared_ptr<ecs::Filter> _filter = nullptr;

    std::shared_ptr<ecs::Pool<CPlayerTag>> _playerTagPool = nullptr;
    std::shared_ptr<ecs::Pool<CShape>> _shapePool = nullptr;
    std::shared_ptr<ecs::Pool<CDrawable>> _drawablePool = nullptr;
    std::shared_ptr<ecs::Pool<CInput>> _inputPool = nullptr;
    std::shared_ptr<ecs::Pool<CMoveSpeed>> _moveSpeedPool = nullptr;
    std::shared_ptr<ecs::Pool<CTransform>> _transformPool = nullptr;
    std::shared_ptr<ecs::Pool<CVelocity>> _velocityPool = nullptr;
    std::shared_ptr<ecs::Pool<CMoveAcceleration>> _moveAccelerationPool = nullptr;
    std::shared_ptr<ecs::Pool<CSpinSpeed>> _spinSpeedPool = nullptr;
    std::shared_ptr<ecs::Pool<CCollider>> _colliderPool = nullptr;

public:
    SpawnPlayerSystem(std::shared_ptr<Config> config)
    : _config(std::move(config))
    {
    }

    void init(ecs::World& world) override {
        _playerTagPool = world.pool<CPlayerTag>();
        _transformPool = world.pool<CTransform>();
        _velocityPool = world.pool<CVelocity>();
        _shapePool = world.pool<CShape>();
        _inputPool = world.pool<CInput>();
        _moveSpeedPool = world.pool<CMoveSpeed>();
        _moveAccelerationPool = world.pool<CMoveAcceleration>();
        _spinSpeedPool = world.pool<CSpinSpeed>();
        _colliderPool = world.pool<CCollider>();
        _drawablePool = world.pool<CDrawable>();

        _filter = world.buildFilter()
                .include<CPlayerTag>()
                .build();
    }

    void run(ecs::World& world) override {
        if (_filter->entities().empty()) {
            createNewPlayer(world);
        }
    }

private:

    void createNewPlayer(ecs::World& world) {
        auto entity = world.newEntity();

        auto position = Vector2 (floor(_config->window.width), floor(_config->window.height)) * .5f;

        _playerTagPool->add(entity);
        _transformPool->add(entity, CTransform(position));

        auto shape = createShape(position);
        _shapePool->add(entity, {shape });
        _drawablePool->add(entity, {shape });

        _inputPool->add(entity);
        _velocityPool->add(entity);

        _spinSpeedPool->add(entity, { _config->player.spinSpeed });
        _moveSpeedPool->add(entity, { _config->player.moveSpeed });
        _moveAccelerationPool->add(entity, { _config->player.moveAcceleration });
        _colliderPool->add(entity, { _config->player.radius });
    }

    std::shared_ptr<sf::ConvexShape> createShape(Vector2 position) {
        auto shape = std::make_shared<sf::ConvexShape>();
        shape->setPointCount(4);

        shape->setPoint(0, sf::Vector2f(0.f, -1.f) * _config->player.radius);
        shape->setPoint(1, sf::Vector2f(0.7f, 1.f) * _config->player.radius);
        shape->setPoint(2, sf::Vector2f(0.f, 0.5f) * _config->player.radius);
        shape->setPoint(3, sf::Vector2f(-0.7f, 1.f) * _config->player.radius);

        shape->setOrigin(0.5f, 0.5f);

        shape->setPosition(position());
        shape->setFillColor(_config->player.fillColor());
        shape->setOutlineColor(_config->player.outlineColor());
        shape->setOutlineThickness(_config->player.outlineThickness);

        return shape;
    }
};

#endif //ECS_SPAWN_PLAYER_SYSTEM_H
