//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_COOLDOWN_TICK_SYSTEM_H
#define ECS_COOLDOWN_TICK_SYSTEM_H

#include "../components/components.h"

#include "../../data/color.h"
#include "../../data/vector2.h"
#include "../../ecs/systems.h"

class CooldownTickSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    std::shared_ptr<ecs::Filter> _filter = nullptr;

    std::shared_ptr<ecs::Pool<CCooldown>> _cooldownPool = nullptr;
public:

    void init(ecs::World& world) override {
        _cooldownPool = world.pool<CCooldown>();

        _filter = world.buildFilter()
                .include<CCooldown>()
                .build();
    }

    void run(ecs::World& world) override {
        for (const auto & entity : _filter->entities()) {
            auto & cooldown = _cooldownPool->get(entity);

            // todo: replace with delta time
            cooldown.current -= 1;

            if (cooldown.current <= 0) {
                _cooldownPool->del(entity);
            }
        }
    }
};

#endif //ECS_COOLDOWN_TICK_SYSTEM_H
