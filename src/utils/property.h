//
// Created by Anton Kukhlevskyi on 2024-02-08.
//

#ifndef INTROECS_PROPERTY_H
#define INTROECS_PROPERTY_H

template <typename T>
class Property {
private:
    T _value;

public:
    Property() : _value() {};

    Property(T& value)
    : _value(value)
    {
    }

    Property<T>& operator = (const T& val)
    {
        _value = val;
        return *this;
    };  // eo operator =

    operator const T&() const
    {
        return _value;
    };

    operator T&() const
    {
        return _value;
    };
};


#endif //INTROECS_UTILS_H
