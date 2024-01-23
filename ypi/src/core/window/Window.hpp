/*
 * Filename: Window.hpp
 * Path: ypi/src/ypi/window
 * Created Date: Monday, December 11th 2023, 7:30:35 pm
 * Author: Basile
 *
 * Copyright (c) 2023 Your Company
 */

#ifndef EXNG_WINDOW_HPP_
#define EXNG_WINDOW_HPP_

#include "ypi/lib_headers/common.hpp"
#include "core/vector2/Vector2.hpp"

namespace exng {

    class View;

    // Encapsulate the sfml render window
    class Window
    {
    public:
        Window();
        ~Window();

        // Create the window
        void create(const std::string& title, const Vector2u& size, bool fullscreen = false);

        bool pollEvent(sf::Event& event);

        // Destroy the window
        void close();

        // Clear the window
        void clear();

        // Draw a drawable object
        void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);

        void setFramerateLimit(unsigned int limit);
        void setVerticalSyncEnabled(bool enabled);

        exng::Vector2f getMousePosition() const;

        void setView(const View& view);
        View getView() const;

        // Display the window
        void display();

        // Check if the window is open
        bool isOpen() const;

        // Get the window size
        Vector2u getSize() const;

        Vector2f getCenter() const;

        Vector2f getPosition() const;

        sf::RenderWindow& getHandle();

    private:
        sf::RenderWindow m_window;

    };
} // namespace exng

#endif // EXNG_WINDOW_HPP_