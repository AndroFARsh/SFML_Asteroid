//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_DESTROY_PROJECTILE_SYSTEM_H
#define ECS_DESTROY_PROJECTILE_SYSTEM_H


#include <memory>
#include "../../ecs/systems.h"
#include "../components/components.h"
#include "../config/config.h"

class DestroyProjectileSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    const std::string _name = "DestroyProjectileSystem";

    std::shared_ptr<ecs::Filter> _filter;

public:
    [[nodiscard]] const std::string& name() const override { return _name; }

    void init(ecs::World &world) override {
        _filter = world.buildFilter()
                .include<CProjectileTag>()
                .include<CCollisionHit>()
                .build();
    }

    void run(ecs::World &world, const sf::Time& dt) override {
        for (const auto &entity: _filter->entities()) {
            world.deleteEntity(entity);
        }
    }
};

#endif //ECS_DESTROY_PROJECTILE_SYSTEM_H
