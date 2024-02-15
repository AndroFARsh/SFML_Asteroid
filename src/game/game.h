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
    std::shared_ptr<Config> _config;
    std::shared_ptr<sf::RenderWindow> _window;
    std::shared_ptr<ecs::World> _world;
    std::shared_ptr<ecs::Systems> _systems;


public:
    explicit Game(const std::string& configPath) noexcept;

    void run();
};


#endif //INTROECS_GAME_H
