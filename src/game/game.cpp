//
// Created by Anton Kukhlevskyi on 2024-02-04.
//
#include <memory>

#include "../data/color.h"
#include "../data/vector2.h"

#include "game.h"

#include "systems/input_system.h"
#include "systems/draw_system.h"
#include "systems/spawn_player_system.h"
#include "systems/spawn_asteroid_system.h"
#include "systems/move_player_system.h"
#include "systems/spin_player_system.h"
#include "systems/shoot_player_system.h"
#include "systems/rotate_system.h"
#include "systems/move_system.h"
#include "systems/update_shape_transform_system.h"
#include "systems/cooldown_tick_system.h"
#include "systems/lifespan_tick_system.h"
#include "systems/lifespan_fade_system.h"
#include "systems/collide_system.h"
#include "systems/destroy_asteroid_system.h"
#include "systems/destroy_player_system.h"
#include "systems/destroy_projectile_system.h"
#include "systems/score_system.h"

Game::Game(const std::string& configPath) noexcept
: _config(Config::readFromFile(configPath))
, _window(std::make_shared<sf::RenderWindow>())
, _world(std::make_shared<ecs::World>())
, _systems(nullptr)
{
    _window->create({ _config->window.width, _config->window.height }, _config->window.name, _config->window.style);
    _window->setFramerateLimit(_config->window.frameRate);

    _systems = ecs::SystemsBuilder()
            .add(std::make_shared<InputSystem>(_window))

            .add(std::make_shared<ScoreSystem>(_config))

            .add(std::make_shared<SpawnPlayerSystem>(_config))
            .add(std::make_shared<SpawnAsteroidSystem>(_config))

            .add(std::make_shared<SpinPlayerSystem>())
            .add(std::make_shared<MovePlayerSystem>())
            .add(std::make_shared<ShootPlayerSystem>(_config))

            .add(std::make_shared<RotateSystem>())
            .add(std::make_shared<MoveSystem>())

            // Physics
            .add(std::make_shared<CollideSystem>())
            .add(std::make_shared<DestroyAsteroidSystem>(_config))
            .add(std::make_shared<DestroyPlayerSystem>(_config))
            .add(std::make_shared<DestroyProjectileSystem>())

            .add(std::make_shared<UpdateShapeTransformSystem>(_config))
            .add(std::make_shared<LifespanFadeSystem>())
            .add(std::make_shared<DrawSystem>(_window))

            .add(std::make_shared<CooldownTickSystem>())
            .add(std::make_shared<LifespanTickSystem>())
            .build();
}

void Game::run() {
    _systems->init(*_world);

    while (_window->isOpen()) {
        _systems->run(*_world);
    }

    _systems->dispose(*_world);
}