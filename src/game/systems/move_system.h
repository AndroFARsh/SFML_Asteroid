//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_MOVE_SYSTEM_H
#define ECS_MOVE_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <utility>

#include "../components/components.h"

#include "../../data/color.h"
#include "../../data/vector2.h"
#include "../../ecs/systems.h"
#include "../../utils/utils.h"

#include "../config/config.h"

class MoveSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    std::shared_ptr<ecs::Filter> _filter = nullptr;

    std::shared_ptr<ecs::Pool<CTransform>> _transformPool = nullptr;
    std::shared_ptr<ecs::Pool<CVelocity>> _velocityPool = nullptr;

public:
    void init(ecs::World& world) override {
        _transformPool = world.pool<CTransform>();
        _velocityPool = world.pool<CVelocity>();

        _filter = world.buildFilter()
                .include<CTransform>()
                .include<CVelocity>()
                .build();
    }

    void run(ecs::World& world) override {
        for (auto entity : _filter->entities()) {
            auto & transform = _transformPool->get(entity);

            const auto & velocity = _velocityPool->get(entity);

            transform.position += velocity.value;
        }
    }
};

#endif //ECS_MOVE_SYSTEM_H
