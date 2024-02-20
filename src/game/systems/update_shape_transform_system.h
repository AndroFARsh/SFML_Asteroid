//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_UPDATE_SHAPE_TRANSFORM_SYSTEM_H
#define ECS_UPDATE_SHAPE_TRANSFORM_SYSTEM_H

#include "../components/components.h"

#include "../../data/color.h"
#include "../../data/vector2.h"
#include "../../ecs/systems.h"
#include "../../utils/utils.h"

class UpdateShapeTransformSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    const std::string _name = "UpdateShapeTransformSystem";
    const Config& _config;

    std::shared_ptr<ecs::Filter> _filter = nullptr;

    std::shared_ptr<ecs::Pool<CTransform>> _transformPool = nullptr;
    std::shared_ptr<ecs::Pool<CShape>> _shapePool;

public:
    explicit UpdateShapeTransformSystem(const Config& config)
    : _config(config)
    {
    }

    [[nodiscard]] const std::string& name() const override { return _name; }

    void init(ecs::World& world) override {
        _transformPool = world.pool<CTransform>();
        _shapePool = world.pool<CShape>();

        _filter = world.buildFilter()
                .include<CTransform>()
                .include<CShape>()
                .build();
    }

    void run(ecs::World& world, const sf::Time& dt) override {
        for (auto entity : _filter->entities()) {
            auto &transform = _transformPool->get(entity);
            auto &shape = _shapePool->get(entity);

            if (transform.position.x < 0) transform.position.x += float(_config.window.width);
            if (transform.position.x > float(_config.window.width)) transform.position.x -= float(_config.window.width);

            if (transform.position.y < 0) transform.position.y += float(_config.window.height);
            if (transform.position.y > float(_config.window.height)) transform.position.y -= float(_config.window.height);

            shape.value->setPosition(transform.position());
            shape.value->setRotation(transform.rotation);
            shape.value->setScale(transform.scale, transform.scale);
        }
    }
};

#endif //ECS_UPDATE_SHAPE_TRANSFORM_SYSTEM_H
