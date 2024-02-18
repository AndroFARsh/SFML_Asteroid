//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_DRAW_SYSTEM_H
#define ECS_DRAW_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <memory>
#include "../../ecs/systems.h"

class DrawSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    std::shared_ptr<sf::RenderWindow> _window;

    std::shared_ptr<ecs::Filter> _filter;
    std::shared_ptr<ecs::Pool<CDrawable>> _drawablePool;

public:
    explicit DrawSystem(std::shared_ptr<sf::RenderWindow> window)
    : _window(std::move(window))
    {
    }

    void init(ecs::World& world) override {
        _drawablePool = world.pool<CDrawable>();
        _filter = world.buildFilter()
                .include<CDrawable>()
                .build();
    }

    void run(ecs::World& world) override {
        _window->clear();

        for (const auto & entity : _filter->entities()) {
            const auto & drawable = _drawablePool->get(entity);

            _window->draw(*(drawable.value));
        }

        _window->display();
    }
};

#endif //ECS_DRAW_SYSTEM_H
