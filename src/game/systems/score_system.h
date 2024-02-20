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
    const std::string _name = "ScoreSystem";

    const Config& _config;

    std::shared_ptr<ecs::Filter> _filter = nullptr;

    std::shared_ptr<ecs::Pool<CText>> _textPool = nullptr;
    std::shared_ptr<ecs::Pool<CScore>> _scorePool = nullptr;
    std::shared_ptr<ecs::Pool<CDrawable>> _drawablePool = nullptr;

public:
    explicit ScoreSystem(const Config & config)
    : _config(config)
    {
    }

    [[nodiscard]] const std::string& name() const override { return _name; }

    void init(ecs::World &world) override {
        _textPool = world.pool<CText>();
        _scorePool = world.pool<CScore>();
        _drawablePool = world.pool<CDrawable>();

        auto entity= world.newEntity();
        auto text = createText();

        _textPool->add(entity, { text });
        _drawablePool->add(entity, { text });
        _scorePool->add(entity);

        _filter = world.buildFilter()
                .include<CText>()
                .include<CScore>()
                .build();
    }

    void run(ecs::World &world, const sf::Time& dt) override {
        for (auto entity: _filter->entities()) {
            const auto & score = _scorePool->get(entity);
            auto & text = _textPool->get(entity);

            text.value->setString("Score: " +  std::to_string(score.value));
        }
    }

    std::shared_ptr<sf::Text> createText() {
        auto text = std::make_shared<sf::Text>();

        text->setPosition(20, 20);
        text->setFont(*_config.font.font);
        text->setCharacterSize(_config.font.size);
        text->setFillColor(_config.font.color());

        return text;
    }
};

#endif //ECS_COLLIDE_OBSTACLE_SYSTEM_H
