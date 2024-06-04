/*
 * Filename: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/Rect/Rect.hpp
 * Path: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/Rect
 * Created Date: Tuesday, December 12th 2023, 1:51:11 pm
 * Author: Basile
 * 
 * Copyright (c) 2023 Your Company
 */

#ifndef YPI_RECT_HPP_
#define YPI_RECT_HPP_

#include "Vector2.hpp"
#include "../../lib_headers/sfml.hpp"

namespace ypi {

    template <typename T>
    class Rect : public sf::Rect<T>
    {
    public:
        Rect();
        Rect(T left, T top, T width, T height);
        Rect(const Vector2<T>& position, const Vector2<T>& size);
        Rect(const Rect<T>& other);
        Rect(const sf::Rect<T>& other);
        ~Rect();

        enum class Corner {
            TopLeft,
            TopRight,
            BottomLeft,
            BottomRight
        };

        T left = 0;
        T top = 0;
        T width = 0;
        T height = 0;

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

        Rect<T>& operator=(const sf::Rect<T>& other)
        {
            left = other.left;
            top = other.top;
            width = other.width;
            height = other.height;
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

        bool contains(const T& x, const T& y) const
        {
            return (x >= left && x < left + width && y >= top && y < top + height);
        }

        // multiple the rect size around the center
        Rect<T> multiply(const T& factor) const
        {
            T newWidth = width * factor;
            T newHeight = height * factor;
            T newLeft = left - (newWidth - width) / 2;
            T newTop = top - (newHeight - height) / 2;
            return Rect<T>(newLeft, newTop, newWidth, newHeight);
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

        Rect<T> combine(const Rect<T>& other) const
        {
            auto x1 = std::min(left, other.left);
            auto y1 = std::min(top, other.top);
            auto x2 = std::max(left + width, other.left + other.width);
            auto y2 = std::max(top + height, other.top + other.height);

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

        Vector2<T> getCenter() const
        {
            return Vector2<T>(left + width / 2, top + height / 2);
        }

        template <typename U>
        Rect<U> to() const
        {
            return Rect<U>(static_cast<U>(left), static_cast<U>(top), static_cast<U>(width), static_cast<U>(height));
        }

        Vector2<T> getMiddle() const
        {
            return Vector2<T>(left + width / 2, top + height / 2);
        }

        T getRight() const
        {
            return left + width;
        }

        T getBottom() const
        {
            return top + height;
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
    Rect<T>::Rect(const sf::Rect<T>& other)
        : left(other.left)
        , top(other.top)
        , width(other.width)
        , height(other.height)
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

} // namespace ypi

#endif // YPI_Rect_HPP_