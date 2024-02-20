//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_DESTROY_PLAYER_SYSTEM_H
#define ECS_DESTROY_PLAYER_SYSTEM_H


#include <memory>
#include "../../ecs/systems.h"
#include "../components/components.h"
#include "../config/config.h"

class DestroyPlayerSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    const std::string _name = "DestroyPlayerSystem";
    const Config& _config;

    std::shared_ptr<ecs::Filter> _playerFilter;
    std::shared_ptr<ecs::Filter> _scoreFilter;

    std::shared_ptr<ecs::Pool<CCollisionHit>> _hitPool;

    std::shared_ptr<ecs::Pool<CFragmentTag>> _fragmentTagPool;
    std::shared_ptr<ecs::Pool<CLifespan>> _lifespanPool;
    std::shared_ptr<ecs::Pool<CShape>> _shapePool;
    std::shared_ptr<ecs::Pool<CDrawable>> _drawablePool;
    std::shared_ptr<ecs::Pool<CTransform>> _transformPool;
    std::shared_ptr<ecs::Pool<CVelocity>> _velocityPool;
    std::shared_ptr<ecs::Pool<CRotationVelocity>> _rotationVelocityPool;
    std::shared_ptr<ecs::Pool<CCollider>> _colliderPool;

    std::shared_ptr<ecs::Pool<CScore>> _scorePool;

public:
    explicit DestroyPlayerSystem(const Config& config)
    : _config(config)
    {
    }

    [[nodiscard]] const std::string& name() const override { return _name; }

    void init(ecs::World &world) override {
        _hitPool = world.pool<CCollisionHit>();

        _colliderPool = world.pool<CCollider>();
        _fragmentTagPool = world.pool<CFragmentTag>();
        _lifespanPool = world.pool<CLifespan>();
        _shapePool = world.pool<CShape>();
        _drawablePool = world.pool<CDrawable>();
        _transformPool = world.pool<CTransform>();
        _velocityPool = world.pool<CVelocity>();
        _rotationVelocityPool = world.pool<CRotationVelocity>();

        _scorePool = world.pool<CScore>();

        _playerFilter = world.buildFilter()
                .include<CPlayerTag>()
                .include<CCollisionHit>()
                .build();

        _scoreFilter = world.buildFilter()
                .include<CScore>()
                .build();
    }

    void run(ecs::World &world, const sf::Time& dt) override {
        for (const auto &entity: _playerFilter->entities()) {
            spawnFragment(world, entity);
            updateScore();
            world.deleteEntity(entity);
        }
    }

    void updateScore() {
        for (const auto &scoreEntity: _scoreFilter->entities()) {
            auto &score = _scorePool->get(scoreEntity);

            score.value = 0;
        }
    }

    void spawnFragment(ecs::World &world, const ecs::Entity &asteroidEntity) {
        const auto &hit = _hitPool->get(asteroidEntity);
        const auto &transform = _transformPool->get(asteroidEntity);
        const auto &collider = _colliderPool->get(asteroidEntity);

        auto otherVelocityNormalized = hit.velocity.normalized();
        auto angleStep = 360 / 8;
        for (int i = 0; i < 8; ++i) {
            auto entity = world.newEntity();

            auto forward = otherVelocityNormalized.rotate(float(i * angleStep));
            auto position = transform.position + forward * collider.value;
            auto shape = createShape(position);

            _fragmentTagPool->add(entity);
            _shapePool->add(entity, { shape });
            _drawablePool->add(entity, { shape });
            _transformPool->add(entity, CTransform(position));
            _lifespanPool->add(entity, {_config.projectile.lifespan });
            _velocityPool->add(entity, {forward * _config.fragment.speed });
            _rotationVelocityPool->add(entity, {_config.fragment.rotationSpeed});
        }
    }

    std::shared_ptr<sf::CircleShape> createShape(Vector2 position) {
        auto shape = std::make_shared<sf::CircleShape>(3, 3);
        shape->setPosition(position());
        shape->setFillColor(_config.fragment.fillColor());
        shape->setOutlineColor(_config.fragment.outlineColor());
        shape->setOutlineThickness(_config.fragment.outlineThickness);
        shape->setOrigin(0.5f, 0.5f);
        return shape;
    }
};

#endif //ECS_DESTROY_PLAYER_SYSTEM_H
