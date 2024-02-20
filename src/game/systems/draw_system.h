//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_DRAW_SYSTEM_H
#define ECS_DRAW_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "../../ecs/systems.h"

class DrawSystem : public ecs::IInitSystem, public ecs::IRenderSystem {
private:
    const std::string _name = "DrawSystem";

    sf::RenderWindow& _window;

    std::shared_ptr<ecs::Filter> _filter;
    std::shared_ptr<ecs::Pool<CDrawable>> _drawablePool;

public:
    explicit DrawSystem(sf::RenderWindow& window)
    : _window(window)
    {
    }

    [[nodiscard]] const std::string& name() const override { return _name; }

    void init(ecs::World& world) override {
        _drawablePool = world.pool<CDrawable>();
        _filter = world.buildFilter()
                .include<CDrawable>()
                .build();
    }

    void render(ecs::World& world) override {
        for (const auto & entity : _filter->entities()) {
            const auto & drawable = _drawablePool->get(entity);
            if (drawable.value) {
                _window.draw(*(drawable.value));
            }
        }
    }
};

#endif //ECS_DRAW_SYSTEM_H
