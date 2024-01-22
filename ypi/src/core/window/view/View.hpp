/*
 * Filename: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/window/view/View.hpp
 * Path: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/window/view
 * Created Date: Tuesday, December 12th 2023, 2:19:27 pm
 * Author: Basile
 *
 * Copyright (c) 2023 Your Company
 */

#ifndef EXNG_VIEW_HPP
#define EXNG_VIEW_HPP

#include "core/transform/Transform.hpp"
#include "core/rect/Rect.hpp"
#include "core/window/Window.hpp"

namespace exng {

    class View
    {
        public:
            View();
            ~View();
            void setCenter(const Vector2f& center);
            void setCenter(float x, float y);
            void setSize(const Vector2f& size);
            void setSize(float width, float height);
            void setRotation(float angle);
            void setViewport(const FloatRect& viewport);
            void setViewport(float left, float top, float width, float height);
            void reset(const FloatRect& rectangle);
            void move(const Vector2f& offset);
            void move(float offsetX, float offsetY);
            void rotate(float angle);
            void zoom(float factor);
            const Vector2f getCenter() const;
            const Vector2f getSize() const;
            const FloatRect getViewport() const;
            float getRotation() const;
        private:
            sf::View m_view;
            Transform m_transform;
    };
}

#endif /* !EXNG_VIEW_HPP */