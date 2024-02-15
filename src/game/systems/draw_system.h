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

    std::shared_ptr<ecs::Filter> _shapeFilter;
    std::shared_ptr<ecs::Filter> _textFilter;

    std::shared_ptr<ecs::Pool<CText>> _textPool;
    std::shared_ptr<ecs::Pool<CShape>> _shapePool;

public:
    explicit DrawSystem(std::shared_ptr<sf::RenderWindow> window)
    : _window(std::move(window))
    {
    }

    void init(ecs::World& world) override {
        _textPool = world.pool<CText>();
        _shapePool = world.pool<CShape>();

        _shapeFilter = world.buildFilter()
                .include<CShape>()
                .build();

        _textFilter = world.buildFilter()
                .include<CText>()
                .build();
    }

    void run(ecs::World& world) override {
        _window->clear();

        for (const auto & entity : _shapeFilter->entities()) {
            const auto & shape = _shapePool->get(entity);

            _window->draw(*(shape.value));
        }

        for (const auto & entity : _textFilter->entities()) {
            const auto & text = _textPool->get(entity);

            _window->draw(*(text.value));
        }

        _window->display();
    }
};

#endif //ECS_DRAW_SYSTEM_H
