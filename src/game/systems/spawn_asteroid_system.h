//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_SPAWN_ASTEROID_SYSTEM_H
#define ECS_SPAWN_ASTEROID_SYSTEM_H

#include "../components/components.h"

#include "../../data/color.h"
#include "../../data/vector2.h"
#include "../../ecs/systems.h"
#include "../../utils/utils.h"

#include "../config/config.h"

class SpawnAsteroidSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    std::shared_ptr<Config> _config;

    std::shared_ptr<ecs::Filter> _spawnFilter = nullptr;
    std::shared_ptr<ecs::Filter> _asteroidFilter = nullptr;

    std::shared_ptr<ecs::Pool<CAsteroidSpawnerTag>> _asteroidSpawnerTagPool = nullptr;
    std::shared_ptr<ecs::Pool<CCooldown>> _cooldownPool = nullptr;

    std::shared_ptr<ecs::Pool<CRotationVelocity>> _rotationVelocityPool = nullptr;
    std::shared_ptr<ecs::Pool<CAsteroidTag>> _asteroidTagPool = nullptr;
    std::shared_ptr<ecs::Pool<CTransform>> _transformPool = nullptr;
    std::shared_ptr<ecs::Pool<CVelocity>> _velocityPool = nullptr;
    std::shared_ptr<ecs::Pool<CCollider>> _colliderPool = nullptr;
    std::shared_ptr<ecs::Pool<CShape>> _shapePool = nullptr;
    std::shared_ptr<ecs::Pool<CDrawable>> _drawablePool = nullptr;
    std::shared_ptr<ecs::Pool<CMass>> _massPool = nullptr;

public:
    SpawnAsteroidSystem(std::shared_ptr<Config> config)
    : _config(std::move(config))
    {
    }

    void init(ecs::World& world) override {
        _asteroidSpawnerTagPool = world.pool<CAsteroidSpawnerTag>();
        _cooldownPool = world.pool<CCooldown>();

        _asteroidTagPool = world.pool<CAsteroidTag>();
        _rotationVelocityPool = world.pool<CRotationVelocity>();
        _transformPool = world.pool<CTransform>();
        _velocityPool = world.pool<CVelocity>();
        _colliderPool = world.pool<CCollider>();
        _shapePool = world.pool<CShape>();
        _drawablePool = world.pool<CDrawable>();
        _massPool = world.pool<CMass>();

        _asteroidFilter = world.buildFilter()
                .include<CAsteroidTag>()
                .build();

        _spawnFilter = world.buildFilter()
                .include<CAsteroidSpawnerTag>()
                .exclude<CCooldown>()
                .build();


        auto spawnEntity = world.newEntity();
        _asteroidSpawnerTagPool->add(spawnEntity);
    }

    void run(ecs::World& world) override {
        for (auto entity : _spawnFilter->entities()) {
            _cooldownPool->add(entity, { _config->gameplay.spawnCooldown });
            if (_asteroidFilter->entities().size() < _config->gameplay.spawnMaxAlive) {
                createNewObstacle(world);
            }
        }
    }

    void createNewObstacle(ecs::World& world) {
        auto entity = world.newEntity();

        auto fMass = random(_config->asteroid.massMin, _config->asteroid.massMax);
        auto mass = uint(fMass);
        auto diff = (fMass - _config->asteroid.massMin) / (_config->asteroid.massMax - _config->asteroid.massMin);
        auto rotation = random(_config->asteroid.rotationSpeedMin, _config->asteroid.rotationSpeedMax);

        auto radius = _config->asteroid.baseRadius + 20.f * diff;
        auto speed = _config->asteroid.baseSpeed - 1.2f * diff;

        auto position = createPosition();
        auto velocity = createVelocity(position) * speed;
        auto shape = createShape(radius, mass, position);

        _asteroidTagPool->add(entity);
        _transformPool->add(entity, CTransform(position));
        _shapePool->add(entity, { shape });
        _drawablePool->add(entity, { shape });
        _velocityPool->add(entity, { velocity  });
        _colliderPool->add(entity, { radius });
        _rotationVelocityPool->add(entity, { rotation });
        _massPool->add(entity, { mass });
    }

    Vector2 createVelocity(const Vector2& position) {
        return (Vector2T(
                random(0.2f * float(_config->window.width), 0.8f * float(_config->window.width)),
                random(0.2f * float(_config->window.height), 0.8f * float(_config->window.height))
        ) - position).normalized();
    }

    Vector2 createPosition() {
        auto screen = Vector2(float(_config->window.width), float(_config->window.width));
        auto center = screen * 0.5f;
        auto position = Vector2T(
                random(0.01f * float(_config->window.width), 0.99f * float(_config->window.width)),
                random(0.01f * float(_config->window.height), 0.99f * float(_config->window.height))
        );

        // left-top corner
        if (position.x < center.x && position.y < center.y) {
            return position.x - position.y > 0 ? Vector2(position.x, 0) : Vector2(0, position.y);
        }

        // right-top corner
        if (position.x >= center.x && position.y < center.y) {
            return (screen.x - position.x) - position.y > 0 ? Vector2(position.x, 0) : Vector2(0, position.y);
        }

        // left-bottom corner
        if (position.x < center.x && position.y >= center.y) {
            return position.x - (screen.y - position.y) > 0 ? Vector2(position.x, 0) : Vector2(0, position.y);
        }

        // right-bottom corner
        if (position.x >= center.x && position.y >= center.y) {
            return (screen.x - position.x) - (screen.y - position.y) > 0 ? Vector2(position.x, 0) : Vector2(0, position.y);
        }

        return position;
    }

    std::shared_ptr<sf::ConvexShape> createShape(float radius, uint pointCount, Vector2 position) {
        auto shape = std::make_shared<sf::ConvexShape>();
        shape->setPointCount(pointCount);

        auto angleStep = 360.f / float (pointCount);
        for (auto i = 0; i < pointCount; ++i) {
            auto point = Vector2(0, 1).rotate(angleStep * float(i)) * random(radius * 0.7f, radius * 1.3f);
            shape->setPoint(i, point());
        }

        shape->setPosition(position());
        shape->setFillColor(_config->asteroid.fillColor());
        shape->setOutlineColor(_config->asteroid.outlineColor());
        shape->setOutlineThickness(_config->asteroid.outlineThickness);
        shape->setOrigin(0.5f, 0.5f);
        return shape;
    }
};

#endif //ECS_SPAWN_ASTEROID_SYSTEM_H
