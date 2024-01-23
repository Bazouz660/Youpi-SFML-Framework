/*
** EPITECH PROJECT, 2023
** RPG3
 *  Modified by: Clément Thomas
 *  Modified time: 2023-07-10 03:24:20
*/

#pragma once

#include "ypi/lib_headers/common.hpp"
#include "core/window/Window.hpp"
#include "core/window/view/View.hpp"

namespace exng
{
    namespace context
    {
        static Vector2u targetResolution = {1920, 1080};

        static void setTargetResolution(const Vector2u& resolution)
        {
            targetResolution = resolution;
        }

        static void setTargetResolution(unsigned int x, unsigned int y)
        {
            targetResolution = {x, y};
        }

        static Vector2u getTargetResolution()
        {
            return targetResolution;
        }

    }
}
