//
// Created by Anton Kukhlevskyi on 2024-02-04.
//

#ifndef ECS_DEV_GUI_SYSTEM_H
#define ECS_DEV_GUI_SYSTEM_H

#include <imgui.h>
#include <imgui-SFML.h>

#include "../components/components.h"

#include "../../data/color.h"
#include "../../data/vector2.h"
#include "../../ecs/systems.h"
#include "../../utils/utils.h"

#include "../config/config.h"

class DevGuiSystem :
        public ecs::IInitSystem,
        public ecs::IEventSystem,
        public ecs::IRunSystem,
        public ecs::IRenderSystem,
        public ecs::IDisposeSystem {
private:
    const std::string _name = "DevGuiSystem";

    std::shared_ptr<ecs::Pool<CPlayerTag>> _playerTagPool;
    std::shared_ptr<ecs::Pool<CAsteroidTag>> _asteroidTagPool;
    std::shared_ptr<ecs::Pool<CProjectileTag>> _projectileTagPool;
    std::shared_ptr<ecs::Pool<CFragmentTag>> _fragmentTagPool;

    std::shared_ptr<ecs::Filter> _asteroidsFilter;
    std::shared_ptr<ecs::Filter> _projectileFilter;
    std::shared_ptr<ecs::Filter> _fragmentFilter;
    std::shared_ptr<ecs::Filter> _playerFilter;

    sf::RenderWindow &_window;
    ecs::Systems &_systems;

    bool _initialized = false;

    void renderSystemsSection() {
        if (ImGui::CollapsingHeader("Systems")) {
            for (auto &pair: _systems.systemsWithStatus()) {
                auto name = pair.first;
                auto enabled = &(pair.second);

                ImGui::Checkbox(name.c_str(), enabled);
            }
        }
    }

    void renderEntitySection(ecs::World &world) {
        if (ImGui::CollapsingHeader("Entities")) {
            if (ImGui::TreeNode("All:")) {
                for (auto &entity: world.entities()) {
                    renderEntity(world, entity);
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Player:")) {
                for (auto &entity: _playerFilter->entities()) {
                    renderEntity(world, entity);
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Asteroids:")) {
                for (auto &entity: _asteroidsFilter->entities()) {
                    renderEntity(world, entity);
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Projectile:")) {
                for (auto &entity: _projectileFilter->entities()) {
                    renderEntity(world, entity);
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Fragment:")) {
                for (auto &entity: _fragmentFilter->entities()) {
                    renderEntity(world, entity);
                }
                ImGui::TreePop();
            }
        }
    }

    void renderEntity(ecs::World &world, const ecs::Entity& entity) {
        auto col32 = ImColor::HSV(0.9f, 0.7f, 0.7f);
        if (_playerTagPool->has(entity)) {
            col32.SetHSV(0.1f, 0.7f, 0.7f);
        } else if (_asteroidTagPool->has(entity)) {
            col32.SetHSV(0.3f, 0.7f, 0.7f);
        } else if (_projectileTagPool->has(entity)) {
            col32.SetHSV(0.5f, 0.7f, 0.7f);
        } else if (_fragmentTagPool->has(entity)) {
            col32.SetHSV(0.7f, 0.7f, 0.7f);
        }

        const float sz = 10.0f;
        const ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x+sz, p.y+sz), col32, 3.0f);
        ImGui::Text("  Entity %d", entity);
    }

public:
    DevGuiSystem(sf::RenderWindow &window, ecs::Systems &systems)
            : _window(window), _systems(systems) {}

    [[nodiscard]] const std::string &name() const override { return _name; }

    void init(ecs::World &world) override {
        _initialized = ImGui::SFML::Init(_window, true);

        _playerTagPool = world.pool<CPlayerTag>();
        _asteroidTagPool = world.pool<CAsteroidTag>();
        _projectileTagPool = world.pool<CProjectileTag>();
        _fragmentTagPool = world.pool<CFragmentTag>();

        _asteroidsFilter = world.buildFilter()
                .include<CAsteroidTag>()
                .build();
        _playerFilter = world.buildFilter()
                .include<CPlayerTag>()
                .build();
        _projectileFilter = world.buildFilter()
                .include<CProjectileTag>()
                .build();
        _fragmentFilter = world.buildFilter()
                .include<CFragmentTag>()
                .build();
    }

    void event(ecs::World &world, const sf::Event &event) override {
        if (!_initialized) return;

        ImGui::SFML::ProcessEvent(_window, event);
    }

    void run(ecs::World &world, const sf::Time &dt) override {
        if (!_initialized) return;

        ImGui::SFML::Update(_window, dt);

        // ImGui::ShowDemoWindow();

        ImGui::Begin("Debug Window");
        renderSystemsSection();
        renderEntitySection(world);
        ImGui::End();
    }

    void render(ecs::World &world) override {
        if (!_initialized) return;

        ImGui::SFML::Render(_window);
    }

    void dispose(ecs::World &world) override {
        if (!_initialized) return;

        ImGui::SFML::Shutdown();
        _initialized = false;
    }

};

#endif //ECS_DEV_GUI_SYSTEM_H
