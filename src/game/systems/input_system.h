//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_INPUT_SYSTEM_H
#define ECS_INPUT_SYSTEM_H

#include <memory>

#include "../components/components.h"
#include "../../ecs/systems.h"

class InputSystem : public ecs::IInitSystem, public ecs::IEventSystem {
private:
    const std::string _name = "InputSystem";

    std::shared_ptr<ecs::Filter> _filter;

    std::shared_ptr<ecs::Pool<CInput>> _inputPool;

public:
    [[nodiscard]] const std::string& name() const override { return _name; }

    void init(ecs::World& world) override {
        _inputPool = world.pool<CInput>();

        _filter = world.buildFilter()
                .include<CPlayerTag>()
                .include<CInput>()
                .build();
    }

    void event(ecs::World &world, const sf::Event &event) override {
        if (event.type == sf::Event::KeyPressed) { updateInput(event, true); }
        if (event.type == sf::Event::KeyReleased)  { updateInput(event, false); }
    }

    void updateInput(const sf::Event &event, bool value) {
        for (auto entity: _filter->entities()) {
            auto &input = _inputPool->get(entity);

            switch (event.key.code) {
                case sf::Keyboard::A:
                case sf::Keyboard::Left:
                    input.left = value;
                    break;

                case sf::Keyboard::W:
                case sf::Keyboard::Up:
                    input.up = value;
                    break;

                case sf::Keyboard::D:
                case sf::Keyboard::Right:
                    input.right = value;
                    break;

                case sf::Keyboard::S:
                case sf::Keyboard::Down:
                    input.down = value;
                    break;

                case sf::Keyboard::Space:
                    input.shoot = value;
                    break;
                default:
                    break;
            }
        }
    }
};

#endif //ECS_INPUT_SYSTEM_H
