/*
 * Filename: Window.hpp
 * Path: ypi/src/ypi/window
 * Created Date: Monday, December 11th 2023, 7:30:35 pm
 * Author: Basile
 *
 * Copyright (c) 2023 Your Company
 */

#ifndef YPI_WINDOW_HPP_
#define YPI_WINDOW_HPP_

#include "../../../lib_headers/common.hpp"
#include "../../math/Vector2.hpp"
#include "RenderTarget.hpp"

namespace ypi {

    // Encapsulate the sfml render window
    class RenderWindow : public sf::RenderWindow
    {
    public:
        explicit RenderWindow();

        explicit RenderWindow(sf::VideoMode mode, const std::string& title, sf::Uint32 style = sf::Style::Default, const sf::ContextSettings& settings = sf::ContextSettings());
        ~RenderWindow();

        void create(sf::VideoMode mode, const sf::String& title, sf::Uint32 style);
        void create(sf::VideoMode mode, const sf::String& title, sf::Uint32 style, const sf::ContextSettings& settings);

        Vector2f getMousePosition() const;
        Vector2f getCenter() const;

        void display();
        void clear(const sf::Color& color = sf::Color::Black);

    protected:
        sf::Sprite m_sprite;
    };
} // namespace ypi

#endif // YPI_WINDOW_HPP_