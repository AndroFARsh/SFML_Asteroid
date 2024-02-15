//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_LIFESPAN_FADE_SYSTEM_H
#define ECS_LIFESPAN_FADE_SYSTEM_H

#include "../components/components.h"

#include "../../data/color.h"
#include "../../data/vector2.h"
#include "../../ecs/systems.h"

class LifespanFadeSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    std::shared_ptr<ecs::Filter> _filter = nullptr;

    std::shared_ptr<ecs::Pool<CLifespan>> _lifespanPool = nullptr;
    std::shared_ptr<ecs::Pool<CShape>> _shapePool = nullptr;
public:

    void init(ecs::World& world) override {
        _lifespanPool = world.pool<CLifespan>();
        _shapePool = world.pool<CShape>();

        _filter = world.buildFilter()
                .include<CLifespan>()
                .include<CShape>()
                .build();
    }

    void run(ecs::World& world) override {
        for (const auto & entity : _filter->entities()) {
            auto & shape = _shapePool->get(entity);
            auto & lifespan = _lifespanPool->get(entity);

            auto tick = sf::Uint8(255 / lifespan.total);

            auto fillColor = shape.value->getFillColor();
            auto outerColor = shape.value->getOutlineColor();

            fillColor.a = std::clamp(fillColor.a - tick, 0, 255);
            outerColor.a = std::clamp(fillColor.a - tick, 0, 255);

            shape.value->setFillColor(fillColor);
            shape.value->setOutlineColor(outerColor);
        }
    }
};

#endif //ECS_LIFESPAN_FADE_SYSTEM_H
