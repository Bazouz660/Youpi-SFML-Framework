/*
 * Filename: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/transform/Transform.hpp
 * Path: /home/basile/Projects/Ecole/Current/B-CPP-500-STG-5-2-rtype-julian.lambert/ypi/src/ypi/transform
 * Created Date: Tuesday, December 12th 2023, 1:43:54 pm
 * Author: Basile
 *
 * Copyright (c) 2023 Your Company
 */

#ifndef EXNG_TRANSFORM_HPP_
#define EXNG_TRANSFORM_HPP_

#include "ypi/lib_headers/sfml.hpp"
#include "Vector2.hpp"
#include "core/rect/Rect.hpp"

namespace exng {

    class Transform
    {
        public:

            static const Transform Identity;

            Transform();
            ~Transform();

            Transform& combine(const Transform& other);

            Transform& translate(const Vector2f& offset);
            Transform& translate(float x, float y);

            Transform& rotate(float angle);

            Transform& scale(const Vector2f& factors);
            Transform& scale(float x, float y);

            Transform getInverse() const;

            Vector2f transformPoint(const Vector2f& point) const;
            Vector2f transformPoint(float x, float y) const;

            FloatRect transformRect(const FloatRect& Rect) const;
        private:
            sf::Transform m_transform;
    };

}

#endif /* !EXNG_TRANSFORM_HPP_ */