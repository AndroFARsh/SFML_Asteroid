//
// Created by Anton Kukhlevskyi on 2024-02-14.
//

#include <memory>
#include "config.h"

#include "../../ini/ini_config.h"

Config
Config::readFromFile(const std::string &configPath) {
    IniConfig iniConfig(configPath);

    std::shared_ptr<sf::Font> font = std::make_shared<sf::Font>();
    font->loadFromFile(iniConfig.get("Font", "path", "./sofachrome.otf"));

    return {
            .window {
                    iniConfig.get("Window", "name", ""),
                    iniConfig.get("Window", "width", 800u),
                    iniConfig.get("Window", "height", 600u),
                    iniConfig.get("Window", "frame_rate", 60u),
                    iniConfig.get("Window", "fullscreen", false) ? sf::Style::Fullscreen : sf::Style::Default
            },
            .font {
                    font,
                    iniConfig.get("Font", "size", 24u),
                    iniConfig.get("Font", "color", Color(255, 255, 255)),
            },
            .gameplay {
                    iniConfig.get("Gameplay", "spawn_cooldown", 100.0f),
                    iniConfig.get("Gameplay", "spawn_max_alive", 10u),
            },
            .player {
                    iniConfig.get("Player", "radius", 10.0f),
                    iniConfig.get("Player", "fill_color", Color(255, 200, 200)),
                    iniConfig.get("Player", "outline_color", Color(255, 200, 200)),
                    iniConfig.get("Player", "outline_thickness", 2.0f),
                    iniConfig.get("Player", "move_speed", 2.0f),
                    iniConfig.get("Player", "move_acceleration", 0.01f),
                    iniConfig.get("Player", "spin_speed", 0.02f),
                    iniConfig.get("Player", "shoot_cooldown", 60.f)
            },
            .asteroid {
                    iniConfig.get("Asteroid", "base_radius", 10.0f),
                    iniConfig.get("Asteroid", "base_speed", 3.0f),
                    iniConfig.get("Asteroid", "mass_min", 4.f),
                    iniConfig.get("Asteroid", "mass_max", 10.f),
                    iniConfig.get("Asteroid", "rotation_speed_min", 0.5f),
                    iniConfig.get("Asteroid", "rotation_speed_max", 1.5f),
                    iniConfig.get("Asteroid", "fill_color", Color(255, 200, 200)),
                    iniConfig.get("Asteroid", "outline_color", Color(255, 200, 200)),
                    iniConfig.get("Asteroid", "outline_thickness", 3.0f),
            },
            .projectile {
                    iniConfig.get("Projectile", "radius", 5.f),
                    iniConfig.get("Projectile", "speed", 4.f),
                    iniConfig.get("Projectile", "lifespan", 60.f),
                    iniConfig.get("Projectile", "fill_color", Color(255, 200, 200)),

            },
            .fragment {
                    iniConfig.get("Fragment", "radius", 5.f),
                    iniConfig.get("Fragment", "speed", 4.f),
                    iniConfig.get("Fragment", "spin_speed", 4.f),
                    iniConfig.get("Fragment", "fill_color", Color(255, 200, 200)),
                    iniConfig.get("Fragment", "outline_color", Color(255, 200, 200)),
                    iniConfig.get("Fragment", "outline_thickness", 60.f),
                    iniConfig.get("Fragment", "lifespan", 60.f),
            }
    };
}