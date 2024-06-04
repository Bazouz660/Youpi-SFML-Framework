/*
 * Filename: Vector2.hpp
 * Path: ypi/src/ypi/vector2
 * Created Date: Monday, December 11th 2023, 7:35:55 pm
 * Author: Basile
 *
 * Copyright (c) 2023 Your Company
 */

#ifndef YPI_VECTOR2_HPP_
#define YPI_VECTOR2_HPP_

#include "../../lib_headers/sfml.hpp"
#include <iostream>
#include <string>

namespace ypi {

    template <typename T>
    class Vector2 : public sf::Vector2<T>
    {
    public:
        Vector2() : sf::Vector2<T>() {}
        Vector2(T x, T y) : sf::Vector2<T>(x, y) {}
        Vector2(const Vector2<T>& other) : sf::Vector2<T>(other.x, other.y) {}
        Vector2(const sf::Vector2<T>& other) : sf::Vector2<T>(other.x, other.y) {}
        Vector2(const std::initializer_list<T>& list) : sf::Vector2<T>(*list.begin(), *(list.begin() + 1)) {} // Ajout de ce constructeur
        ~Vector2() = default;

        template <typename U>
        Vector2<U> to() const
        {
            return Vector2<U>(static_cast<U>(this->x), static_cast<U>(this->y));
        }
    };

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Vector2<T>& vec)
    {
        os << "(" << vec.x << ", " << vec.y << ")";
        return os;
    }

    typedef Vector2<int> Vector2i;
    typedef Vector2<unsigned int> Vector2u;
    typedef Vector2<float> Vector2f;

} // namespace ypi

#endif // YPI_VECTOR2_HPP_