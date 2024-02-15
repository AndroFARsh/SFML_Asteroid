//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_LIFESPAN_TICK_SYSTEM_H
#define ECS_LIFESPAN_TICK_SYSTEM_H

#include "../components/components.h"

#include "../../data/color.h"
#include "../../data/vector2.h"
#include "../../ecs/systems.h"

class LifespanTickSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    std::shared_ptr<ecs::Filter> _filter = nullptr;

    std::shared_ptr<ecs::Pool<CLifespan>> _lifespanPool = nullptr;
public:

    void init(ecs::World& world) override {
        _lifespanPool = world.pool<CLifespan>();

        _filter = world.buildFilter()
                .include<CLifespan>()
                .build();
    }

    void run(ecs::World& world) override {
        for (const auto & entity : _filter->entities()) {
            auto & lifespan = _lifespanPool->get(entity);

            // todo: replace with delta time
            lifespan.current -= 1;

            if (lifespan.current <= 0) {
                world.deleteEntity(entity);
            }
        }
    }
};

#endif //ECS_LIFESPAN_TICK_SYSTEM_H
