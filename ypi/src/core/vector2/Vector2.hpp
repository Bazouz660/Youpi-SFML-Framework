/*
 * Filename: Vector2.hpp
 * Path: ypi/src/ypi/vector2
 * Created Date: Monday, December 11th 2023, 7:35:55 pm
 * Author: Basile
 *
 * Copyright (c) 2023 Your Company
 */

#ifndef EXNG_VECTOR2_HPP_
#define EXNG_VECTOR2_HPP_

#include <iostream>
#include <string>

namespace exng {

    template <typename T>
    class Vector2
    {
    public:
        Vector2();
        Vector2(T x, T y);
        Vector2(const Vector2<T>& other);
        ~Vector2();

        T x;
        T y;

        // Assignment operator
        Vector2<T>& operator=(const Vector2<T>& other)
        {
            if (this != &other) {
                x = other.x;
                y = other.y;
            }
            return *this;
        }

        // Addition operator
        Vector2<T> operator+(const Vector2<T>& other) const
        {
            return Vector2<T>(x + other.x, y + other.y);
        }

        // Subtraction operator
        Vector2<T> operator-(const Vector2<T>& other) const
        {
            return Vector2<T>(x - other.x, y - other.y);
        }

        // Multiplication operator (scalar)
        Vector2<T> operator*(T scalar) const
        {
            return Vector2<T>(x * scalar, y * scalar);
        }

        // Division operator (scalar)
        Vector2<T> operator/(T scalar) const
        {
            return Vector2<T>(x / scalar, y / scalar);
        }

        // Equality operator
        bool operator==(const Vector2<T>& other) const
        {
            return (x == other.x) && (y == other.y);
        }

        // Inequality operator
        bool operator!=(const Vector2<T>& other) const
        {
            return !(*this == other);
        }

        Vector2<T>& operator+=(const Vector2<T>& other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2<T>& operator-=(const Vector2<T>& other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vector2<T>& operator*=(T scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Vector2<T>& operator/=(T scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }
    };

    template <typename T>
    Vector2<T> operator*(T scalar, const Vector2<T>& vec)
    {
        return Vector2<T>(vec.x * scalar, vec.y * scalar);
    }

    template <typename T>
    Vector2<T> operator/(T scalar, const Vector2<T>& vec)
    {
        return Vector2<T>(vec.x / scalar, vec.y / scalar);
    }

    template <typename T>
    Vector2<T> operator-(const Vector2<T>& vec)
    {
        return Vector2<T>(-vec.x, -vec.y);
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Vector2<T>& vec)
    {
        os << "(" << vec.x << ", " << vec.y << ")";
        return os;
    }

    template <typename T>
    Vector2<T>::Vector2()
        : x(0), y(0)
    {
    }

    template <typename T>
    Vector2<T>::Vector2(T x, T y)
        : x(x), y(y)
    {
    }

    template <typename T>
    Vector2<T>::Vector2(const Vector2<T>& other)
        : x(other.x), y(other.y)
    {
    }

    template <typename T>
    Vector2<T>::~Vector2()
    {
    }

    typedef Vector2<int> Vector2i;
    typedef Vector2<unsigned int> Vector2u;
    typedef Vector2<float> Vector2f;

} // namespace exng

#endif // EXNG_VECTOR2_HPP_