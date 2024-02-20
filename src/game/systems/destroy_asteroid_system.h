//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_DESTROY_ASTEROID_SYSTEM_H
#define ECS_DESTROY_ASTEROID_SYSTEM_H


#include <memory>
#include "../../ecs/systems.h"
#include "../components/components.h"

class DestroyAsteroidSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    const std::string _name = "DestroyAsteroidSystem";

    const Config& _config;

    std::shared_ptr<ecs::Filter> _asteroidFilter;
    std::shared_ptr<ecs::Filter> _scoreFilter;


    std::shared_ptr<ecs::Pool<CCollisionHit>> _hitPool;
    std::shared_ptr<ecs::Pool<CMass>> _massPool;

    std::shared_ptr<ecs::Pool<CScore>> _scorePool;
    std::shared_ptr<ecs::Pool<CShape>> _shapePool;
    std::shared_ptr<ecs::Pool<CDrawable>> _drawablePool;
    std::shared_ptr<ecs::Pool<CFragmentTag>> _fragmentTagPool;
    std::shared_ptr<ecs::Pool<CLifespan>> _lifespanPool;
    std::shared_ptr<ecs::Pool<CTransform>> _transformPool;
    std::shared_ptr<ecs::Pool<CVelocity>> _velocityPool;
    std::shared_ptr<ecs::Pool<CRotationVelocity>> _rotationVelocityPool;
    std::shared_ptr<ecs::Pool<CCollider>> _colliderPool;

public:
    explicit DestroyAsteroidSystem(const Config& config)
    : _config(config)
    {
    }

    [[nodiscard]] const std::string& name() const override { return _name; }

    void init(ecs::World &world) override {
        _hitPool = world.pool<CCollisionHit>();
        _massPool = world.pool<CMass>();

        _colliderPool = world.pool<CCollider>();
        _drawablePool = world.pool<CDrawable>();
        _fragmentTagPool = world.pool<CFragmentTag>();
        _lifespanPool = world.pool<CLifespan>();
        _shapePool = world.pool<CShape>();
        _transformPool = world.pool<CTransform>();
        _velocityPool = world.pool<CVelocity>();
        _rotationVelocityPool = world.pool<CRotationVelocity>();

        _scorePool = world.pool<CScore>();

        _asteroidFilter = world.buildFilter()
                .include<CAsteroidTag>()
                .include<CCollisionHit>()
                .build();

        _scoreFilter = world.buildFilter()
                .include<CScore>()
                .build();
    }

    void run(ecs::World &world, const sf::Time& dt) override {
        for (const auto &asteroidEntity: _asteroidFilter->entities()) {
            const auto &hit = _hitPool->get(asteroidEntity);
            if (hit.type == Asteroid) {
                bounceAsteroid(asteroidEntity);
                _hitPool->del(asteroidEntity);
            } else {
                spawnFragment(world, asteroidEntity);
                updateScore(asteroidEntity);
                world.deleteEntity(asteroidEntity);
            }
        }
    }

    void updateScore(const ecs::Entity &asteroidEntity) {
        for (const auto &scoreEntity: _scoreFilter->entities()) {
            const auto &mass = _massPool->get(asteroidEntity);
            auto &score = _scorePool->get(scoreEntity);

            score.value += mass.value * 10;
        }
    }

    void bounceAsteroid(const ecs::Entity &asteroidEntity) {
        const auto &hit = _hitPool->get(asteroidEntity);

        auto &velocity = _velocityPool->get(asteroidEntity);

        auto velocityValue = velocity.value.magnitude();
        auto newVelocity = hit.position.normalized() * velocityValue;

        velocity.value = newVelocity;
    }

    void spawnFragment(ecs::World &world, const ecs::Entity &asteroidEntity) {
        const auto &hit = _hitPool->get(asteroidEntity);
        const auto &mass = _massPool->get(asteroidEntity);
        const auto &transform = _transformPool->get(asteroidEntity);
        const auto &collider = _colliderPool->get(asteroidEntity);

        auto otherVelocityNormalized = hit.velocity.normalized();
        auto angleStep = 360.f / float(mass.value);
        for (int i = 0; i < mass.value; ++i) {
            auto entity = world.newEntity();

            auto forward = otherVelocityNormalized.rotate(float(i) * angleStep);
            auto position = transform.position + forward * collider.value;
            auto shape = createShape(position);

            _fragmentTagPool->add(entity);
            _shapePool->add(entity, { shape });
            _drawablePool->add(entity, { shape });
            _transformPool->add(entity, CTransform(position));
            _lifespanPool->add(entity, {_config.projectile.lifespan});
            _velocityPool->add(entity, {forward * _config.fragment.speed});
            _rotationVelocityPool->add(entity, {_config.fragment.rotationSpeed});
        }
    }

    std::shared_ptr<sf::CircleShape> createShape(Vector2 position) {
        auto shape = std::make_shared<sf::CircleShape>(_config.fragment.radius, random(3, 5));
        shape->setPosition(position());
        shape->setFillColor(_config.fragment.fillColor());
        shape->setOutlineColor(_config.fragment.outlineColor());
        shape->setOutlineThickness(_config.fragment.outlineThickness);
        shape->setOrigin(0.5f, 0.5f);
        return shape;
    }


};

#endif //ECS_DESTROY_ASTEROID_SYSTEM_H
