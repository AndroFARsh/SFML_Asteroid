//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_ROTATE_SYSTEM_H
#define ECS_ROTATE_SYSTEM_H

#include "../components/components.h"

#include "../../data/color.h"
#include "../../data/vector2.h"
#include "../../ecs/systems.h"
#include "../../utils/utils.h"

class RotateSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    const std::string _name = "RotateSystem";

    std::shared_ptr<ecs::Filter> _filter = nullptr;
    std::shared_ptr<ecs::Pool<CTransform>> _transformPool = nullptr;
    std::shared_ptr<ecs::Pool<CRotationVelocity>> _rotationVelocityPool = nullptr;

public:
    [[nodiscard]] const std::string& name() const override { return _name; }

    void init(ecs::World& world) override {
        _transformPool = world.pool<CTransform>();
        _rotationVelocityPool = world.pool<CRotationVelocity>();

        _filter = world.buildFilter()
                .include<CTransform>()
                .include<CRotationVelocity>()
                .build();
    }

    void run(ecs::World& world, const sf::Time& dt) override {
        for (auto entity : _filter->entities()) {
            auto & transform = _transformPool->get(entity);
            const auto & _rotationVelocity = _rotationVelocityPool->get(entity);

            transform.rotation += _rotationVelocity.value;

            if (transform.rotation > 360) { transform.rotation -= 360; }
            if (transform.rotation < 0) { transform.rotation += 360; }
        }
    }
};

#endif //ECS_ROTATE_SYSTEM_H
