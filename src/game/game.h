//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef INTROECS_GAME_H
#define INTROECS_GAME_H

#include <SFML/Graphics.hpp>
#include <memory>

#include "../ecs/world.h"
#include "../ecs/systems.h"
#include "config/config.h"

class Game {
private:
    Config _config {};
    sf::Event _event {};
    sf::Clock _deltaClock {};

    sf::RenderWindow _window;

    ecs::World _world;
    ecs::Systems _systems;

public:
    explicit Game(const std::string& configPath) noexcept;

    void run();
};


#endif //INTROECS_GAME_H
