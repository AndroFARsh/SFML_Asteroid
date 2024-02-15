//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_INPUT_SYSTEM_H
#define ECS_INPUT_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "../components/components.h"
#include "../../ecs/systems.h"

class InputSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    std::shared_ptr<sf::RenderWindow> _window;
    std::shared_ptr<ecs::Filter> _filter;

    std::shared_ptr<ecs::Pool<CInput>> _inputPool = nullptr;

public:
    InputSystem(std::shared_ptr<sf::RenderWindow> window)
    : _window(std::move(window))
    {}

    void init(ecs::World& world) override {
        _inputPool = world.pool<CInput>();

        _filter = world.buildFilter()
                .include<CPlayerTag>()
                .include<CInput>()
                .build();
    }

    void run(ecs::World& world) override {
        for (auto event = sf::Event{}; _window->pollEvent(event);) {
            if (event.type == sf::Event::Closed) {
                _window->close();
            }

            if(event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
                for(auto entity : _filter->entities()) {
                    auto & input = _inputPool->get(entity);

                    // change state if press or release Left button
                    if ((!input.left && isPressed(event, {sf::Keyboard::A, sf::Keyboard::Left})) ||
                        (input.left && isReleased(event, {sf::Keyboard::A, sf::Keyboard::Left}))) { input.left = !input.left; }

                    // change state if press or release Up button
                    if ((!input.up && isPressed(event, {sf::Keyboard::W, sf::Keyboard::Up})) ||
                        (input.up && isReleased(event, {sf::Keyboard::W, sf::Keyboard::Up}))) { input.up = !input.up; }

                    // change state if press or release Right button
                    if ((!input.right && isPressed(event, {sf::Keyboard::D, sf::Keyboard::Right})) ||
                        (input.right && isReleased(event, {sf::Keyboard::D, sf::Keyboard::Right}))) { input.right = !input.right; }

                    // change state if press or release Down button
                    if ((!input.down && isPressed(event, {sf::Keyboard::S, sf::Keyboard::Down})) ||
                        (input.down && isReleased(event, {sf::Keyboard::S, sf::Keyboard::Down}))) { input.down = !input.down; }

                    // change state if press or release Shoot button
                    if ((!input.shoot && isPressed(event, {sf::Keyboard::Space})) ||
                        (input.shoot && isReleased(event, {sf::Keyboard::Space}))) { input.shoot = !input.shoot; }

                }
            }
        }
    }
private:
    static bool isPressed(sf::Event event, const std::vector<sf::Keyboard::Key>& keys) {
        if(event.type == sf::Event::KeyPressed) {
            for (auto k : keys) {
                if (event.key.code == k)
                    return true;
            }
        }
        return false;
    }

    static bool isReleased(sf::Event event, const std::vector<sf::Keyboard::Key>& keys) {
        if(event.type == sf::Event::KeyReleased) {
            for (auto & k : keys) {
                if (event.key.code == k)
                    return true;
            }
        }
        return false;
    }
};

#endif //ECS_INPUT_SYSTEM_H
