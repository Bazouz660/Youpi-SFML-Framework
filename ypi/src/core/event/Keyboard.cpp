/*
 * Filename: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/event/Keyboard.cpp
 * Path: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/event
 * Created Date: Tuesday, December 12th 2023, 4:13:49 pm
 * Author: Basile
 *
 * Copyright (c) 2023 Your Company
 */

#include "Keyboard.hpp"

namespace exng
{
    bool Keyboard::isKeyPressed(Key key)
    {
        return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key));
    }
}