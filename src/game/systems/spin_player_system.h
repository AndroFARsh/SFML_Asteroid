//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_SPIN_PLAYER_SYSTEM_H
#define ECS_SPIN_PLAYER_SYSTEM_H

#include "../components/components.h"

#include "../../data/color.h"
#include "../../data/vector2.h"
#include "../../ecs/systems.h"
#include "../../utils/utils.h"

class SpinPlayerSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    std::shared_ptr<ecs::Filter> _filter = nullptr;

    std::shared_ptr<ecs::Pool<CTransform>> _transformPool = nullptr;
    std::shared_ptr<ecs::Pool<CInput>> _inputPool = nullptr;
    std::shared_ptr<ecs::Pool<CSpinSpeed>> _speedPool = nullptr;

public:
    void init(ecs::World& world) override {
        _transformPool = world.pool<CTransform>();
        _speedPool = world.pool<CSpinSpeed>();
        _inputPool = world.pool<CInput>();

        _filter = world.buildFilter()
                .include<CPlayerTag>()
                .include<CTransform>()
                .include<CSpinSpeed>()
                .include<CInput>()
                .build();
    }

    void run(ecs::World& world) override {
        for (auto entity : _filter->entities()) {
            auto & transform = _transformPool->get(entity);

            const auto & speed= _speedPool->get(entity);
            const auto & input = _inputPool->get(entity);

            auto rotateAcceleration = speed.value * ((input.left ? -1.0f : 0.0f) + (input.right ? 1.0f : 0.0f));

            auto angle = transform.rotation + rotateAcceleration;
            transform.rotation = angle;
        }
    }
};

#endif //ECS_SPIN_PLAYER_SYSTEM_H
