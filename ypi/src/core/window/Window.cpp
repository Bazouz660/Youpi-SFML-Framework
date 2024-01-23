/*
 * Filename: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/window/Window.cpp
 * Path: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/window
 * Created Date: Monday, December 11th 2023, 7:30:42 pm
 * Author: Basile
 * 
 * Copyright (c) 2023 Your Company
 */

#include "Window.hpp"
#include "core/window/view/View.hpp"

namespace exng {

    Window::Window()
    {
    }

    Window::~Window()
    {
    }

    void Window::create(const std::string& title, const Vector2u& size, bool fullscreen)
    {
        auto style = fullscreen ? sf::Style::Fullscreen : sf::Style::Titlebar | sf::Style::Close;
        m_window.create(sf::VideoMode(size.x, size.y), title, style);
    }

    void Window::close()
    {
        m_window.close();
    }

    void Window::clear()
    {
        m_window.clear();
    }

    void Window::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
    {
        m_window.draw(drawable, states);
    }

    void Window::display()
    {
        m_window.display();
    }

    bool Window::isOpen() const
    {
        return m_window.isOpen();
    }

    Vector2u Window::getSize() const
    {
        auto size = m_window.getSize();
        return Vector2u(size.x, size.y);
    }

    Vector2f Window::getCenter() const
    {
        auto size = m_window.getSize();
        return Vector2f(size.x / 2, size.y / 2);
    }

    Vector2f Window::getPosition() const
    {
        auto pos = m_window.getPosition();
        return Vector2f(pos.x, pos.y);
    }

    bool Window::pollEvent(sf::Event& event)
    {
        return m_window.pollEvent(event);
    }

    sf::RenderWindow& Window::getHandle()
    {
        return m_window;
    }

    void Window::setFramerateLimit(unsigned int limit)
    {
        m_window.setFramerateLimit(limit);
    }

    void Window::setVerticalSyncEnabled(bool enabled)
    {
        m_window.setVerticalSyncEnabled(enabled);
    }

    void Window::setView(const View& view)
    {
        sf::View sfmlView;
        sfmlView.setCenter(view.getCenter().x, view.getCenter().y);
        sfmlView.setSize(view.getSize().x, view.getSize().y);
        sfmlView.setRotation(view.getRotation());
        sfmlView.setViewport(sf::FloatRect(view.getViewport().left, view.getViewport().top, view.getViewport().width, view.getViewport().height));
        m_window.setView(sfmlView);
    }

    View Window::getView() const
    {
        auto sfmlView = m_window.getView();
        View view;
        view.setCenter(sfmlView.getCenter().x, sfmlView.getCenter().y);
        view.setSize(sfmlView.getSize().x, sfmlView.getSize().y);
        view.setRotation(sfmlView.getRotation());
        view.setViewport(FloatRect(sfmlView.getViewport().left, sfmlView.getViewport().top, sfmlView.getViewport().width, sfmlView.getViewport().height));
        return view;
    }

    Vector2f Window::getMousePosition() const
    {
        auto pos = sf::Mouse::getPosition(m_window);
        // transform the mouse position from window coordinates to world coordinates
        auto worldPos = m_window.mapPixelToCoords(pos);
        return Vector2f(worldPos.x, worldPos.y);
    }

} // namespace exng