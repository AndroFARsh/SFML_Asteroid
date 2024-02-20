#ifndef ECS_SYSTEMS_H
#define ECS_SYSTEMS_H

#include <memory>
#include <vector>

#include "world.h"

namespace ecs {

    class ISystem {
    public:
        [[nodiscard]] virtual const std::string& name() const = 0;
    };

    class IPreInitSystem : public virtual ISystem {
    public:
        virtual void preInit(World& world) = 0;
    };

    class IInitSystem : public virtual ISystem {
    public:
        virtual void init(World& world) = 0;
    };

    class IEventSystem : public virtual ISystem {
    public:
        virtual void event(World& world, const sf::Event& event) = 0;
    };

    class IRenderSystem : public virtual ISystem {
    public:
        virtual void render(World& world) = 0;
    };

    class IRunSystem : public virtual ISystem {
    public:
        virtual void run(World& world, const sf::Time& dt) = 0;
    };

    class IDisposeSystem : public virtual ISystem {
    public:
        virtual void dispose(World& world) = 0;
    };

    class IPostDisposeSystem : public virtual ISystem {
    public:
        virtual void postDispose(World& world) = 0;
    };

    class Systems {
    private:
        class SystemsBuilder {
        private:
            friend class Systems;

            std::map<std::string, bool> _systems;
            std::vector<std::shared_ptr<IPreInitSystem>> _preInitSystems;
            std::vector<std::shared_ptr<IInitSystem>> _initSystems;
            std::vector<std::shared_ptr<IEventSystem>> _eventSystems;
            std::vector<std::shared_ptr<IRunSystem>> _runSystems;
            std::vector<std::shared_ptr<IRenderSystem>> _renderSystems;
            std::vector<std::shared_ptr<IDisposeSystem>> _disposeSystems;
            std::vector<std::shared_ptr<IPostDisposeSystem>> _postDisposeSystems;

        public:
            template <typename T>
            SystemsBuilder &add(const std::shared_ptr<T> &s) {
                auto preInitSystem = std::dynamic_pointer_cast<IPreInitSystem>(s);
                if (preInitSystem) {
                    _systems[preInitSystem->name()] = true;
                    _preInitSystems.push_back(preInitSystem);
                }

                auto initSystem = std::dynamic_pointer_cast<IInitSystem>(s);
                if (initSystem) {
                    _systems[initSystem->name()] = true;
                    _initSystems.push_back(initSystem);
                }

                auto eventSystem = std::dynamic_pointer_cast<IEventSystem>(s);
                if (eventSystem) {
                    _systems[eventSystem->name()] = true;
                    _eventSystems.push_back(eventSystem);
                }

                auto runSystem = std::dynamic_pointer_cast<IRunSystem>(s);
                if (runSystem) {
                    _systems[runSystem->name()] = true;
                    _runSystems.push_back(runSystem);
                }

                auto renderSystem = std::dynamic_pointer_cast<IRenderSystem>(s);
                if (renderSystem) {
                    _systems[renderSystem->name()] = true;
                    _renderSystems.push_back(renderSystem);
                }

                auto disposeSystem = std::dynamic_pointer_cast<IDisposeSystem>(s);
                if (disposeSystem) {
                    _systems[disposeSystem->name()] = true;
                    _disposeSystems.push_back(disposeSystem);
                }

                auto postDisposeSystem = std::dynamic_pointer_cast<IPostDisposeSystem>(s);
                if (postDisposeSystem) {
                    _systems[postDisposeSystem->name()] = true;
                    _postDisposeSystems.push_back(postDisposeSystem);
                }

                return *this;
            }

            Systems build() {
                return {
                        _systems,
                        _preInitSystems,
                        _initSystems,
                        _eventSystems,
                        _runSystems,
                        _renderSystems,
                        _disposeSystems,
                        _postDisposeSystems
                };
            }
        };

        std::map<std::string, bool> _systemsWithStatus;
        std::vector<std::shared_ptr<IPreInitSystem>> _preInitSystems;
        std::vector<std::shared_ptr<IInitSystem>> _initSystems;
        std::vector<std::shared_ptr<IEventSystem>> _eventSystems;
        std::vector<std::shared_ptr<IRunSystem>> _runSystems;
        std::vector<std::shared_ptr<IRenderSystem>> _renderSystems;
        std::vector<std::shared_ptr<IDisposeSystem>> _disposeSystems;
        std::vector<std::shared_ptr<IPostDisposeSystem>> _postDisposeSystems;

        Systems(
                std::map<std::string, bool> systemsWithStatus,
                std::vector<std::shared_ptr<IPreInitSystem>> preInitSystems,
                std::vector<std::shared_ptr<IInitSystem>> initSystems,
                std::vector<std::shared_ptr<IEventSystem>> eventSystems,
                std::vector<std::shared_ptr<IRunSystem>> runSystems,
                std::vector<std::shared_ptr<IRenderSystem>> renderSystems,
                std::vector<std::shared_ptr<IDisposeSystem>> disposeSystems,
                std::vector<std::shared_ptr<IPostDisposeSystem>> postDisposeSystems
        )
        : _systemsWithStatus(std::move(systemsWithStatus))
        , _preInitSystems(std::move(preInitSystems))
        , _initSystems(std::move(initSystems))
        , _eventSystems(std::move(eventSystems))
        , _runSystems(std::move(runSystems))
        , _renderSystems(std::move(renderSystems))
        , _disposeSystems(std::move(disposeSystems))
        , _postDisposeSystems(std::move(postDisposeSystems))
        {}

        [[nodiscard]] bool isSystemEnabled(const std::string& name) const {
            auto it = _systemsWithStatus.find(name);
            return it != _systemsWithStatus.end() && it->second;
        }

    public:
        std::map<std::string, bool>& systemsWithStatus() { return _systemsWithStatus; }

        void init(World& world) const {
            for (const auto& preInitSystem : _preInitSystems) {
                if (preInitSystem && isSystemEnabled(preInitSystem->name()))
                    preInitSystem->preInit(world);
            }
            world.update();

            for (const auto& initSystem : _initSystems) {
                if (initSystem && isSystemEnabled(initSystem->name()))
                    initSystem->init(world);
            }
            world.update();
        }

        void event(World& world, const sf::Event& event) {
            for (const auto& eventSystem : _eventSystems) {
                if (eventSystem && isSystemEnabled(eventSystem->name()))
                    eventSystem->event(world, event);
            }
            world.update();
        }

        void run(World& world, const sf::Time& dt) {
            for (const auto& runSystems : _runSystems) {
                if (runSystems && isSystemEnabled(runSystems->name()))
                    runSystems->run(world, dt);
            }
            world.update();
        }

        void render(World& world) {
            for (const auto& renderSystem : _renderSystems) {
                if (renderSystem && isSystemEnabled(renderSystem->name()))
                    renderSystem->render(world);
            }
            world.update();
        }

        void dispose(World& world) {
            for (const auto& disposeSystem : _disposeSystems) {
                if (disposeSystem && isSystemEnabled(disposeSystem->name()))
                    disposeSystem->dispose(world);
            }
            world.update();

            for (const auto& postDisposeSystem : _postDisposeSystems) {
                if (postDisposeSystem && isSystemEnabled(postDisposeSystem->name()))
                    postDisposeSystem->postDispose(world);
            }
            world.update();
        }

        static SystemsBuilder builder() { return {}; }
    };
}

#endif //ECS_SYSTEMS_H
