/*
 * Filename: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/transform/Transform.cpp
 * Path: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/transform
 * Created Date: Tuesday, December 12th 2023, 1:43:59 pm
 * Author: Basile
 *
 * Copyright (c) 2023 Your Company
 */

#include "Transform.hpp"

namespace ypi {

    const Transform Transform::Identity;

    Transform::Transform()
    {
    }

    Transform::~Transform()
    {
    }

    Transform& Transform::combine(const Transform& other)
    {
        m_transform.combine(other.m_transform);
        return *this;
    }

    Transform& Transform::translate(const Vector2f& offset)
    {
        m_transform.translate(offset.x, offset.y);
        return *this;
    }

    Transform& Transform::translate(float x, float y)
    {
        m_transform.translate(x, y);
        return *this;
    }

    Transform& Transform::rotate(float angle)
    {
        m_transform.rotate(angle);
        return *this;
    }

    Transform& Transform::scale(const Vector2f& factors)
    {
        m_transform.scale(factors.x, factors.y);
        return *this;
    }

    Transform& Transform::scale(float x, float y)
    {
        m_transform.scale(x, y);
        return *this;
    }

    Transform Transform::getInverse() const
    {
        Transform transform;
        transform.m_transform = m_transform.getInverse();
        return transform;
    }

    Vector2f Transform::transformPoint(const Vector2f& point) const
    {
        auto vec = m_transform.transformPoint(point.x, point.y);
        return Vector2f(vec.x, vec.y);
    }

    Vector2f Transform::transformPoint(float x, float y) const
    {
        auto vec = m_transform.transformPoint(x, y);
        return Vector2f(vec.x, vec.y);
    }

    FloatRect Transform::transformRect(const FloatRect& Rect) const
    {
        sf::FloatRect rect(Rect.left, Rect.top, Rect.width, Rect.height);

        auto vec = m_transform.transformRect(rect);
        return FloatRect(vec.left, vec.top, vec.width, vec.height);
    }
}