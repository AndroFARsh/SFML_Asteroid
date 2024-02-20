//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_MOVE_PLAYER_SYSTEM_H
#define ECS_MOVE_PLAYER_SYSTEM_H

#include "../components/components.h"

#include "../../data/color.h"
#include "../../data/vector2.h"
#include "../../ecs/systems.h"
#include "../../utils/utils.h"

class MovePlayerSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    const std::string _name = "MovePlayerSystem";

    std::shared_ptr<ecs::Filter> _filter = nullptr;

    std::shared_ptr<ecs::Pool<CTransform>> _transformPool = nullptr;
    std::shared_ptr<ecs::Pool<CInput>> _inputPool = nullptr;
    std::shared_ptr<ecs::Pool<CMoveSpeed>> _speedPool = nullptr;
    std::shared_ptr<ecs::Pool<CMoveAcceleration>> _accelerationPool = nullptr;
    std::shared_ptr<ecs::Pool<CVelocity>> _velocityPool = nullptr;

public:
    [[nodiscard]] const std::string& name() const override { return _name; }

    void init(ecs::World& world) override {
        _transformPool = world.pool<CTransform>();
        _speedPool = world.pool<CMoveSpeed>();
        _accelerationPool = world.pool<CMoveAcceleration>();
        _inputPool = world.pool<CInput>();
        _velocityPool = world.pool<CVelocity>();

        _filter = world.buildFilter()
                .include<CPlayerTag>()
                .include<CTransform>()
                .include<CVelocity>()
                .include<CMoveSpeed>()
                .include<CMoveAcceleration>()
                .include<CInput>()
                .build();
    }

    void run(ecs::World& world, const sf::Time& dt) override {
        for (auto entity : _filter->entities()) {
            auto & velocity = _velocityPool->get(entity);

            const auto & transform = _transformPool->get(entity);
            const auto & speed= _speedPool->get(entity);
            const auto & acceleration = _accelerationPool->get(entity);
            const auto & input = _inputPool->get(entity);

            auto moveAcceleration = acceleration.value * ((input.up ? 1.f : 0.0f) + (input.down ? -1.f : 0.0f));
            auto newVelocity = velocity.value + transform.forward() * moveAcceleration;

            velocity.value = newVelocity.normalized() * std::clamp(newVelocity.magnitude(), 0.0f, speed.value);
        }
    }
};

#endif //ECS_MOVE_PLAYER_SYSTEM_H
