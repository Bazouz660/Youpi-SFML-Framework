/*
 * Filename: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/Rect/Rect.hpp
 * Path: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/Rect
 * Created Date: Tuesday, December 12th 2023, 1:51:11 pm
 * Author: Basile
 * 
 * Copyright (c) 2023 Your Company
 */

#ifndef EXNG_RECT_HPP_
#define EXNG_RECT_HPP_

#include "Vector2.hpp"
#include "ypi/lib_headers/sfml.hpp"

namespace exng {

    template <typename T>
    class Rect
    {
    public:
        Rect();
        Rect(T left, T top, T width, T height);
        Rect(const Vector2<T>& position, const Vector2<T>& size);
        Rect(const Rect<T>& other);
        ~Rect();

        enum class Corner {
            TopLeft,
            TopRight,
            BottomLeft,
            BottomRight
        };

        T left;
        T top;
        T width;
        T height;

        // Assignment operator
        Rect<T>& operator=(const Rect<T>& other)
        {
            if (this != &other) {
                left = other.left;
                top = other.top;
                width = other.width;
                height = other.height;
            }
            return *this;
        }

        // Equality operator
        bool operator==(const Rect<T>& other) const
        {
            return (left == other.left && top == other.top && width == other.width && height == other.height);
        }

        // Inequality operator
        bool operator!=(const Rect<T>& other) const
        {
            return !(*this == other);
        }

        // Check if a point is inside the Rectangle's area
        bool contains(const Vector2<T>& point) const
        {
            return (point.x >= left && point.x < left + width && point.y >= top && point.y < top + height);
        }

        // Check intersection between two Rectangles
        bool intersects(const Rect<T>& other) const
        {
            return (left < other.left + other.width && left + width > other.left && top < other.top + other.height && top + height > other.top);
        }

        // Get the intersection between two Rectangles
        Rect<T> getIntersection(const Rect<T>& other) const
        {
            auto x1 = std::max(left, other.left);
            auto y1 = std::max(top, other.top);
            auto x2 = std::min(left + width, other.left + other.width);
            auto y2 = std::min(top + height, other.top + other.height);

            return Rect<T>(x1, y1, x2 - x1, y2 - y1);
        }

        // Get the union between two Rectangles
        Rect<T> getUnion(const Rect<T>& other) const
        {
            auto x1 = std::min(left, other.left);
            auto y1 = std::min(top, other.top);
            auto x2 = std::max(left, other.left);
            auto y2 = std::max(top, other.top);

            return Rect<T>(x1, y1, x2 - x1, y2 - y1);
        }

        Vector2<T> getCorner(Corner corner) const
        {
            switch (corner) {
                case Corner::TopLeft:
                    return Vector2<T>(left, top);
                case Corner::TopRight:
                    return Vector2<T>(left + width, top);
                case Corner::BottomLeft:
                    return Vector2<T>(left, top + height);
                case Corner::BottomRight:
                    return Vector2<T>(left + width, top + height);
            }
            return Vector2<T>();
        }
    };

    template <typename T>
    Rect<T>::Rect()
        : left(0)
        , top(0)
        , width(0)
        , height(0)
    {
    }

    template <typename T>
    Rect<T>::Rect(T left, T top, T width, T height)
        : left(left)
        , top(top)
        , width(width)
        , height(height)
    {
    }

    template <typename T>
    Rect<T>::Rect(const Vector2<T>& position, const Vector2<T>& size)
        : left(position.x)
        , top(position.y)
        , width(size.x)
        , height(size.y)
    {
    }

    template <typename T>
    Rect<T>::Rect(const Rect<T>& other)
        : left(other.left)
        , top(other.top)
        , width(other.width)
        , height(other.height)
    {
    }

    template <typename T>
    Rect<T>::~Rect()
    {
    }

    typedef Rect<int> IntRect;
    typedef Rect<float> FloatRect;

} // namespace exng

#endif // EXNG_Rect_HPP_