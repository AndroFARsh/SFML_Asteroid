#ifndef ECS_SYSTEMS_H
#define ECS_SYSTEMS_H

#include <memory>
#include <vector>

#include "world.h"

namespace ecs {

    class ISystem {
    private:
        bool _enabled = true;

    public:
        virtual bool isEnabled() { return _enabled; }

        void enable(bool enable) { _enabled = enable; }
    };

    class IPreInitSystem : public ISystem {
    public:
        virtual void preInit(World& world) = 0;
    };

    class IInitSystem : public ISystem {
    public:
        virtual void init(World& world) = 0;
    };

    class IRunSystem : public ISystem {
    public:
        virtual void run(World& world) = 0;
    };

    class IDisposeSystem : public ISystem {
    public:
        virtual void dispose(World& world) = 0;
    };

    class IPostDisposeSystem : public ISystem {
    public:
        virtual void postDispose(World& world) = 0;
    };

    class Systems {
    private:
        std::vector<std::shared_ptr<IPreInitSystem>> _preInitSystems;
        std::vector<std::shared_ptr<IInitSystem>> _initSystems;
        std::vector<std::shared_ptr<IRunSystem>> _runSystems;
        std::vector<std::shared_ptr<IDisposeSystem>> _disposeSystems;
        std::vector<std::shared_ptr<IPostDisposeSystem>> _postDisposeSystems;

    public:
        Systems(
            std::vector<std::shared_ptr<IPreInitSystem>> preInitSystems,
            std::vector<std::shared_ptr<IInitSystem>> initSystems,
            std::vector<std::shared_ptr<IRunSystem>> runSystems,
            std::vector<std::shared_ptr<IDisposeSystem>> disposeSystems,
            std::vector<std::shared_ptr<IPostDisposeSystem>> postDisposeSystems
        )
        : _preInitSystems(std::move(preInitSystems))
        , _initSystems(std::move(initSystems))
        , _runSystems(std::move(runSystems))
        , _disposeSystems(std::move(disposeSystems))
        , _postDisposeSystems(std::move(postDisposeSystems))
        {}

        void init(World& world) {
            for (const auto& preInitSystem : _preInitSystems) {
                if (preInitSystem && preInitSystem->isEnabled())
                    preInitSystem->preInit(world);
            }
            world.update();

            for (const auto& initSystem : _initSystems) {
                if (initSystem && initSystem->isEnabled())
                    initSystem->init(world);
            }
            world.update();
        }

        void run(World& world) {
            for (const auto& runSystems : _runSystems) {
                if (runSystems && runSystems->isEnabled())
                    runSystems->run(world);
            }
            world.update();
        }

        void dispose(World& world) {
            for (const auto& disposeSystem : _disposeSystems) {
                if (disposeSystem && disposeSystem->isEnabled())
                    disposeSystem->dispose(world);
            }
            world.update();

            for (const auto& postDisposeSystem : _postDisposeSystems) {
                if (postDisposeSystem && postDisposeSystem->isEnabled())
                    postDisposeSystem->postDispose(world);
            }
            world.update();
        }
    };

    class SystemsBuilder {
    private:
        std::vector<std::shared_ptr<IPreInitSystem>> _preInitSystems;
        std::vector<std::shared_ptr<IInitSystem>> _initSystems;
        std::vector<std::shared_ptr<IRunSystem>> _runSystems;
        std::vector<std::shared_ptr<IDisposeSystem>> _disposeSystems;
        std::vector<std::shared_ptr<IPostDisposeSystem>> _postDisposeSystems;

    public:
        template <typename T>
        SystemsBuilder & add(const std::shared_ptr<T>& system) {
            auto preInitSystem = std::dynamic_pointer_cast<IPreInitSystem>(system);
            if (preInitSystem) {
                _preInitSystems.push_back(preInitSystem);
            }

            auto initSystem = std::dynamic_pointer_cast<IInitSystem>(system);
            if (initSystem) {
                _initSystems.push_back(initSystem);
            }

            auto runSystem = std::dynamic_pointer_cast<IRunSystem>(system);
            if (runSystem) {
                _runSystems.push_back(runSystem);
            }

            auto disposeSystem = std::dynamic_pointer_cast<IDisposeSystem>(system);
            if (disposeSystem) {
                _disposeSystems.push_back(disposeSystem);
            }

            auto postDisposeSystem = std::dynamic_pointer_cast<IPostDisposeSystem>(system);
            if (postDisposeSystem) {
                _postDisposeSystems.push_back(postDisposeSystem);
            }

            return *this;
        }

        std::shared_ptr<Systems> build() {
            return std::make_shared<Systems>(
                _preInitSystems,
                _initSystems,
                _runSystems,
                _disposeSystems,
                _postDisposeSystems
            );
        }
    };
}

#endif //ECS_SYSTEMS_H
