//
// Created by Anton Kukhlevskyi on 2024-02-03.
//

#ifndef ECS_FILTER_H
#define ECS_FILTER_H

#include <utility>
#include <vector>
#include "types.h"

namespace ecs {

    class FilterImpl : public Filter {
    private:
        const IWorldEventListener & _listener;

        std::vector<Type> _include;
        std::vector<Type> _exclude;

        std::set<Entity> _entities;

    public:
        FilterImpl(const IWorldEventListener & listener, std::vector<Type> include, std::vector<Type> exclude)
        : _listener(listener)
        , _include(std::move(include))
        , _exclude(std::move(exclude))
        {
        }

        std::set<Entity>& entities() override { return _entities; }

        void update(const Entity& entity) override {
            if (check(entity)) {
                _entities.insert(entity);
            } else {
                _entities.erase(entity);
            }
        }

    private:
        bool check(const Entity &entity) {
            return std::all_of(
                    _include.begin(),
                    _include.end(),
                    [this, &entity](const Type &type) { return _listener.hasComponent(entity, type); }
            ) && std::all_of(
                    _exclude.begin(),
                    _exclude.end(),
                    [this, &entity](const Type &type) { return !_listener.hasComponent(entity, type); }
            );
        }
    };


    class Mask {
    protected:
        IWorldEventListener & _listener;

        std::vector<Type> _include;
        std::vector<Type> _exclude;

    public:
        explicit Mask(IWorldEventListener & listener)
        : _listener(listener)
        {
        }

        template <typename T>
        Mask& include()
        {
            _include.push_back(createType<T>());
            return *this;
        }

        template <typename T>
        Mask& exclude()
        {
            _exclude.push_back(createType<T>());
            return *this;
        }

        std::shared_ptr<Filter> build() {
            auto filter = std::make_shared<FilterImpl>(
                    _listener,
                    std::move(_include),
                    std::move(_exclude)
            );
            _listener.onFilterCreated(filter);
            return filter;
        }
    };
}

#endif //ECS_FILTER_H
