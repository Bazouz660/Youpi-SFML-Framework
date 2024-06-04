/*
 * Filename: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/window/Window.cpp
 * Path: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/window
 * Created Date: Monday, December 11th 2023, 7:30:42 pm
 * Author: Basile
 *
 * Copyright (c) 2023 Your Company
 */

#include "Window.hpp"
#include "context.hpp"

namespace ypi {

    RenderWindow::RenderWindow()
    {
    }

    void RenderWindow::create(sf::VideoMode mode, const sf::String& title, sf::Uint32 style, const sf::ContextSettings& settings)
    {
        sf::RenderWindow::create(mode, title, style, settings);
        context::setWindow(*this);
        context::setTargetResolution(this->getSize());
        this->setPosition(sf::Vector2i(0, 0));
    }

    void RenderWindow::create(sf::VideoMode mode, const sf::String& title, sf::Uint32 style)
    {
        create(mode, title, style, sf::ContextSettings());
    }

    RenderWindow::RenderWindow(sf::VideoMode mode, const std::string& title, sf::Uint32 style, const sf::ContextSettings& settings)
        : sf::RenderWindow(mode, title, style, settings)
    {
        create(mode, title, style, settings);
    }

    RenderWindow::~RenderWindow()
    {
    }

    Vector2f RenderWindow::getCenter() const
    {
        auto size = sf::WindowBase::getSize();
        return Vector2f(size.x / 2, size.y / 2);
    }

    Vector2f RenderWindow::getMousePosition() const
    {
        auto pos = sf::Mouse::getPosition(*this);
        // transform the mouse position from window coordinates to world coordinates
        auto worldPos = mapPixelToCoords(pos);
        return Vector2f(worldPos.x, worldPos.y);
    }

    void RenderWindow::display()
    {
        sf::RenderWindow::display();
    }

    void RenderWindow::clear(const sf::Color& color)
    {
        sf::RenderWindow::clear(color);
    }

} // namespace ypi
