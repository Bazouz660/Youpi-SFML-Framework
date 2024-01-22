/*
 * Filename: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/window/view/View.cpp
 * Path: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/window/view
 * Created Date: Tuesday, December 12th 2023, 2:19:30 pm
 * Author: Basile
 * 
 * Copyright (c) 2023 Your Company
 */

#include "View.hpp"

namespace exng {

    View::View()
    {
    }

    View::~View()
    {
    }

    void View::setCenter(const Vector2f& center)
    {
        m_view.setCenter(center.x, center.y);
    }

    void View::setCenter(float x, float y)
    {
        m_view.setCenter(x, y);
    }

    void View::setSize(const Vector2f& size)
    {
        m_view.setSize(size.x, size.y);
    }

    void View::setSize(float width, float height)
    {
        m_view.setSize(width, height);
    }

    void View::setRotation(float angle)
    {
        m_view.setRotation(angle);
    }

    void View::setViewport(const FloatRect& viewport)
    {
        m_view.setViewport(sf::FloatRect(viewport.left, viewport.top, viewport.width, viewport.height));
    }

    void View::setViewport(float left, float top, float width, float height)
    {
        m_view.setViewport(sf::FloatRect(left, top, width, height));
    }

    void View::reset(const FloatRect& rectangle)
    {
        m_view.reset(sf::FloatRect(rectangle.left, rectangle.top, rectangle.width, rectangle.height));
    }

    void View::move(const Vector2f& offset)
    {
        m_view.move(offset.x, offset.y);
    }

    void View::move(float offsetX, float offsetY)
    {
        m_view.move(offsetX, offsetY);
    }

    void View::rotate(float angle)
    {
        m_view.rotate(angle);
    }

    void View::zoom(float factor)
    {
        m_view.zoom(factor);
    }

    const Vector2f View::getCenter() const
    {
        auto center = m_view.getCenter();
        return Vector2f(center.x, center.y);
    }

    const Vector2f View::getSize() const
    {
        auto size = m_view.getSize();
        return Vector2f(size.x, size.y);
    }

    const FloatRect View::getViewport() const
    {
        auto viewport = m_view.getViewport();
        return FloatRect(viewport.left,
                            viewport.top,
                            viewport.width,
                            viewport.height);
    }

    float View::getRotation() const
    {
        return m_view.getRotation();
    }

} // namespace exng