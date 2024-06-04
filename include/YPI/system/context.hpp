/*
** EPITECH PROJECT, 2023
** RPG3
 *  Modified by: Clément Thomas
 *  Modified time: 2023-07-10 03:24:20
*/

#pragma once

#include "../../lib_headers/common.hpp"
#include "../graphics/window/Window.hpp"

namespace ypi
{
    namespace context
    {
        inline Vector2u m_targetResolution = {1920, 1080};
        inline RenderWindow* m_window = nullptr;

        static void setTargetResolution(const Vector2u& resolution)
        {
            m_targetResolution = resolution;
        }

        static void setTargetResolution(unsigned int x, unsigned int y)
        {
            m_targetResolution = {x, y};
        }

        static Vector2u getTargetResolution()
        {
            return m_targetResolution;
        }

        static void setWindow(RenderWindow& window)
        {
            m_window = &window;
        }

        static RenderWindow& getWindow()
        {
            if (m_window == nullptr) {
                throw std::runtime_error("Error: context window is not set");
            }
            return *m_window;
        }

        static Vector2i getMousePosition()
        {
            if (m_window == nullptr) {
                return {0, 0};
            }
            return sf::Mouse::getPosition(getWindow());
        }

        static void changeVideoMode(const sf::VideoMode& mode, sf::Uint32 style = sf::Style::Default, sf::ContextSettings settings = sf::ContextSettings())
        {
            if (m_window == nullptr) {
                throw std::runtime_error("Error: context window is not set");
            }
            getWindow().create(mode, "YPI", style, settings);
        }

    }
}
