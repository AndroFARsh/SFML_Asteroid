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
    const std::string _name = "CooldownTickSystem";

    std::shared_ptr<ecs::Filter> _filter;

    std::shared_ptr<ecs::Pool<CCooldown>> _cooldownPool;
public:
    [[nodiscard]] const std::string& name() const override { return _name; }

    void init(ecs::World& world) override {
        _cooldownPool = world.pool<CCooldown>();

        _filter = world.buildFilter()
                .include<CCooldown>()
                .build();
    }

    void run(ecs::World& world, const sf::Time& dt) override {
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
