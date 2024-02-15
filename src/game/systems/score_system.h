//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_SCORE_SYSTEM_H
#define ECS_SCORE_SYSTEM_H

#include "../components/components.h"

#include "../../data/color.h"
#include "../../data/vector2.h"
#include "../../ecs/systems.h"
#include "../../utils/utils.h"

#include "../config/config.h"

class ScoreSystem : public ecs::IInitSystem, public ecs::IRunSystem {
private:
    std::shared_ptr<Config> _config;

    std::shared_ptr<ecs::Filter> _filter = nullptr;

    std::shared_ptr<ecs::Pool<CText>> _textPool = nullptr;
    std::shared_ptr<ecs::Pool<CScore>> _scorePool = nullptr;

public:
    ScoreSystem(std::shared_ptr<Config> config)
    : _config(std::move(config))
    {
    }

    void init(ecs::World &world) override {
        _textPool = world.pool<CText>();
        _scorePool = world.pool<CScore>();

        auto entity= world.newEntity();
        auto text = std::make_shared<sf::Text>();

        text->setPosition(20, 20);
        text->setFont(*_config->font.font);
        text->setCharacterSize(_config->font.size);
        text->setFillColor(_config->font.color());

        _textPool->add(entity, { text });
        _scorePool->add(entity);

        _filter = world.buildFilter()
                .include<CText>()
                .include<CScore>()
                .build();
    }

    void run(ecs::World &world) override {
        for (auto entity: _filter->entities()) {
            const auto & score = _scorePool->get(entity);
            auto & text = _textPool->get(entity);

            text.value->setString("Score: " +  std::to_string(score.value));
        }
    }
};

#endif //ECS_COLLIDE_OBSTACLE_SYSTEM_H
