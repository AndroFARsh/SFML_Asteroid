#ifndef ECS_TYPE_H
#define ECS_TYPE_H

#include <memory>
#include <set>
#include <map>

namespace ecs {

    typedef u_int64_t Entity;

    class Type {
    private:
        const char *_name;
        const size_t _hash;
        const size_t _size;

    public:
        Type() : _name(nullptr), _hash(0), _size(0) {}

        Type(const char *name, size_t hash, size_t size) : _name(name), _hash(hash), _size(size) {}

        Type(const Type &type) : _name(type.name()), _hash(type.hash()), _size(type.size()) {}

        [[nodiscard]] const char *name() const { return _name; }

        [[nodiscard]] size_t hash() const { return _hash; }

        [[nodiscard]] size_t size() const { return _size; }

        bool operator==(const Type &type) const {
            return _size == type.size() && _hash == type.hash() && _name == type.name();
        }

        bool operator!=(const Type &type) const { return !operator==(type); }

        bool operator<(const Type &type) const { return _hash < type.hash(); }
    };

    template<typename T>
    Type createType() {
        const auto &typeInfo = typeid(T);
        return {typeInfo.name(), typeInfo.hash_code(), sizeof(T)};
    }

    class Filter {
    public:
        virtual std::set<Entity> &entities() = 0;

        virtual void update(const Entity &entity) = 0;
    };

    class IWorldEventListener {
    public:
        enum EntityAction {
            ComponentAdded,
            ComponentDeleted
        };

        virtual void onEntityCreated (const Entity& entity) = 0;
        virtual void onEntityChanged (const Entity& entity, const Type& type, EntityAction action) = 0;
        virtual void onEntityDeleted (const Entity& entity) = 0;
        virtual void onFilterCreated (std::shared_ptr<Filter> filter) = 0;

        [[nodiscard]] virtual bool hasComponent(const Entity &entity, const Type &type) const = 0;
    };
}

#endif //ECS_TYPE_H
