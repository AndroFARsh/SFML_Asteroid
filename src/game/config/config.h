//
// Created by Anton Kukhlevskyi on 2024-02-14.
//

#ifndef INTROECS_CONFIG_H
#define INTROECS_CONFIG_H

#include <iostream>
#include "../../data/color.h"

struct Config {
    struct Window {
        std::string name;
        uint width;
        uint height;
        uint frameRate;
        uint32_t style;
    };

    struct Font {
        std::shared_ptr<sf::Font> font;
        uint size;
        Color color;
    };

    struct Gameplay {
        float spawnCooldown;
        uint spawnMaxAlive;
    };

    struct Player {
        float radius;
        Color fillColor;
        Color outlineColor;
        float outlineThickness;
        float moveSpeed;
        float moveAcceleration;
        float spinSpeed;
        float shootCooldown;
    };

    struct Asteroid {
        float baseRadius;
        float baseSpeed;
        float massMin;
        float massMax;
        float rotationSpeedMin;
        float rotationSpeedMax;
        Color fillColor;
        Color outlineColor;
        float outlineThickness;
    };

    struct Projectile {
        float radius;
        float speed;
        float lifespan;
        Color fillColor;
    };

    struct Fragment {
        float radius;
        float speed;
        float rotationSpeed;
        Color fillColor;
        Color outlineColor;
        float outlineThickness;
        float lifespan;
    };

    Window window;
    Font font;
    Gameplay gameplay;
    Player player;
    Asteroid asteroid;
    Projectile projectile;
    Fragment fragment;

    static Config readFromFile(const std::string& iniPath);
};


#endif //INTROECS_CONFIG_H
