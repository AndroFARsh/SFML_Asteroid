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
    std::shared_ptr<ecs::Filter> _filter;

public:
    void init(ecs::World &world) override {
        _filter = world.buildFilter()
                .include<CProjectileTag>()
                .include<CCollisionHit>()
                .build();
    }

    void run(ecs::World &world) override {
        for (const auto &entity: _filter->entities()) {
            world.deleteEntity(entity);
        }
    }
};

#endif //ECS_DESTROY_PROJECTILE_SYSTEM_H
