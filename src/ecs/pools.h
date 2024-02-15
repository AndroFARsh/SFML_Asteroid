//
// Created by Anton Kukhlevskyi on 2024-02-03.
//

#ifndef ECS_POOLS_H
#define ECS_POOLS_H

#include <utility>
#include <vector>
#include "types.h"

namespace ecs {

    class __Pool__ {
    private:
        Type _type;

    public:
        explicit __Pool__(const Type& type) : _type(type) {}

        const Type& type() { return _type; }

        [[nodiscard]] virtual bool has(const Entity& entity) const { return false; }

        virtual void del(const Entity& entity) {
            throw std::runtime_error("Should be override in derived class");
        }
    };

    template <typename T>
    class Pool : public __Pool__ {
    private:
        IWorldEventListener& _listener;
        std::map<Entity, T> _components;

    public:
        explicit Pool(IWorldEventListener& listener)
        : __Pool__(createType<T>())
        , _listener(listener)
        {
        }

        [[nodiscard]] bool has(const Entity& entity) const override {
            return _components.find(entity) != _components.end();
        }

        void add(const Entity& entity) { add(entity, T()); }

        void add(const Entity& entity, const T& component) {
            if (has(entity)) {
                throw std::runtime_error(&"The given component already exist on Entity" [entity]);
            }
            _components.try_emplace(entity, component);
            _listener.onEntityChanged(entity, type(), IWorldEventListener::ComponentAdded);
        }

        T& get(const Entity& entity) {
            if (!has(entity)) {
                throw std::runtime_error(&"No component for Entity" [entity]);
            }

            return _components.find(entity)->second;
        }

        const T& get(const Entity& entity) const {
            if (!has(entity)) {
                throw std::runtime_error(&"No component for Entity" [entity]);
            }
            return _components.find(entity)->second;
        }

        void del(const Entity& entity) override {
            if (!has(entity)) {
                throw std::runtime_error(&"No component for Entity" [entity]);
            }
            _components.erase(entity);
            _listener.onEntityChanged(entity, type(), IWorldEventListener::ComponentDeleted);
        }
    };
}

#endif //ECS_POOLS_H
