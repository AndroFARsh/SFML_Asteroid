#ifndef ECS_WORLD_H
#define ECS_WORLD_H

#include <memory>
#include <set>
#include <map>

#include "types.h"
#include "filter.h"
#include "pools.h"

namespace ecs {

    class World {
    private:
        struct WorldImpl : public IWorldEventListener {
            Entity _lastEntity = 0;
            std::set<Entity> _entities;
            std::set<Entity> _entitiesToDelete;
            std::set<Entity> _entitiesToUpdate;
            std::map<Entity, std::set<Type>> _entityToComponentsTypeSet;

            std::map<Type, std::shared_ptr<__Pool__>> _componentTypeToPool;
            std::vector<std::shared_ptr<Filter>> _filters;

            WorldImpl() = default;

            void onEntityCreated (const Entity& entity) override {
                _entitiesToUpdate.insert(entity);
            };

            void onEntityChanged (const Entity& entity, const Type& type, EntityAction action) override {
                if (action == ComponentAdded) _entityToComponentsTypeSet[entity].insert(type);
                if (action == ComponentDeleted) _entityToComponentsTypeSet[entity].erase(type);

                _entitiesToUpdate.insert(entity);
            };

            void onEntityDeleted (const Entity& entity) override {
                _entitiesToUpdate.insert(entity);
            };

            void onFilterCreated(std::shared_ptr<Filter> filter) override {
                _filters.push_back(filter);
            }

            [[nodiscard]] bool hasComponent(const Entity &entity, const Type &type) const override {
                auto it = _entityToComponentsTypeSet.find(entity);
                if (it != _entityToComponentsTypeSet.end()) {
                    auto &components = it->second;
                    return components.find(type) != components.end();
                }

                return false;
            }

            Entity newEntity() {
                auto entity = ++_lastEntity;

                _entities.insert(entity);
                onEntityCreated(entity);

                return entity;
            }

            void deleteEntity(const Entity &entity) {
                _entitiesToDelete.insert(entity);
                onEntityDeleted(entity);
            }

            Mask buildFilter() {
                return Mask(*this);
            }

            void update() {
                // Delete entities and attached components
                for (auto entity: _entitiesToDelete) {
                    auto componentTypeSet = _entityToComponentsTypeSet[entity];
                    for (const auto &type: componentTypeSet) {
                        auto it = _componentTypeToPool.find(type);
                        if (it != _componentTypeToPool.end()) {
                            auto pool =  it->second;
                            pool->del(entity);
                        }
                    }
                    _entityToComponentsTypeSet.erase(entity);
                }
                _entitiesToDelete.clear();

                // Update filters
                for (auto entity: _entitiesToUpdate) {
                    for (auto & filter: _filters) {
                        filter->update(entity);
                    }
                }
                _entitiesToUpdate.clear();
            }

            template<typename T>
            std::shared_ptr<Pool<T>> pool() {
                const auto & type = createType<T>();
                auto it = _componentTypeToPool.find(type);
                if (it == _componentTypeToPool.end()) {
                    auto pool = std::make_shared<Pool<T>>(*this);
                    _componentTypeToPool.try_emplace(type, std::dynamic_pointer_cast<__Pool__>(pool));
                }
                return std::dynamic_pointer_cast<Pool<T>>(_componentTypeToPool[type]);
            }
        };

        WorldImpl _impl;

    public:
        Entity newEntity() {
            return _impl.newEntity();
        }

        void deleteEntity(const Entity &entity) {
            _impl.deleteEntity(entity);
        }

        Mask buildFilter() {
            return _impl.buildFilter();
        }

        [[nodiscard]] const std::set<Entity> & entities() const {
            return _impl._entities;
        }

        void update() {
            _impl.update();
        }

        template<typename T>
        std::shared_ptr<Pool<T>> pool() {
            return _impl.pool<T>();
        }
    };
}

#endif //ECS_WORLD_H
