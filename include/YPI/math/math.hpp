/*
** EPITECH PROJECT, 2023
** RPG3
** File description:
** math
*/

#pragma once

#include "../../lib_headers/common.hpp"
#include <math.h>
#include "Rect.hpp"
#include "Vector2.hpp"
#include <functional>
#include <unordered_map>

#define PI 3.14159265358979323
#define RD 0.01745329251994329

namespace ypi {
    namespace math {

        static const FloatRect NULL_RECT = FloatRect(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), 0, 0);

        extern float dist(Vector2f p1, Vector2f p2);
        extern float distSquared(Vector2f p1, Vector2f p2);
        extern FloatRect multRect(FloatRect rect, Vector2f n);
        extern Vector2f normalize(Vector2f v);
        extern Vector2f reflect(Vector2f v, Vector2f n);
        extern float dot(Vector2f v1, Vector2f v2);
        extern float det(Vector2f v1, Vector2f v2);
        extern float cross(Vector2f v1, Vector2f v2);
        extern float angle(Vector2f v1, Vector2f v2);
        extern float length(Vector2f v);
        extern float lengthSquared(Vector2f v);
        extern Vector2f unit(Vector2f v);
        extern Vector2f lerp(Vector2f v1, Vector2f v2, float t);
        extern float lerp(float v1, float v2, float t);
        extern float toDeg(float rad);
        extern float toRad(float deg);
        extern float invertAngle(float deg);
        extern bool pointInCircle(Vector2f point, Vector2f center, float radius);

        extern float flip(float x);
        extern float dumbPow(float x, uint32_t p);
        extern float smoothStop(float t, uint32_t power);

        // Function to compute the greatest common divisor (GCD) using Euclidean algorithm
        static int gcd(int a, int b) {
            while (b != 0) {
                int temp = b;
                b = a % b;
                a = temp;
            }
            return a;
        }

        // Function to calculate the aspect ratio in X:Y format
        static Vector2i aspectRatio(int width, int height) {
            // Ensure width and height are positive integers
            if (width <= 0 || height <= 0) {
                throw std::invalid_argument("Width and height must be greater than 0");
            }

            // Calculate the greatest common divisor (GCD) to simplify the ratio
            int divisor = gcd(width, height);

            // Calculate the simplified width and height
            int simplifiedWidth = width / divisor;
            int simplifiedHeight = height / divisor;

            // Return the aspect ratio as a string in the format X:Y
            return Vector2i(simplifiedWidth, simplifiedHeight);
        }

        template <typename T>
        Vector2<T> rotate(Vector2<T> v, float angle)
        {
            float rad = angle * RD;
            float cs = cos(rad);
            float sn = sin(rad);
            return Vector2<T>(v.x * cs - v.y * sn, v.x * sn + v.y * cs);
        }

        template <typename T>
        int sgn(T val) {
            return (T(0) < val) - (val < T(0));
        }

        enum class EasingFunction {
            Linear,
            InQuad,
            OutQuad,
            InOutQuad,
            InCubic,
            OutCubic,
            InOutCubic,
            InQuart,
            OutQuart,
            InOutQuart,
            InQuint,
            OutQuint,
            InOutQuint,
            InSine,
            OutSine,
            InOutSine,
            InExpo,
            OutExpo,
            InOutExpo,
            InCirc,
            OutCirc,
            InOutCirc,
            InBack,
            OutBack,
            InOutBack,
            InElastic,
            OutElastic,
            InOutElastic,
            InBounce,
            OutBounce,
            InOutBounce
        };

        // easing functions
        inline double linear(double t) { return t; }
        inline double easeInQuad(double t) { return t * t; }
        inline double easeOutQuad(double t) { return t * (2 - t); }
        inline double easeInOutQuad(double t) { return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t; }
        inline double easeInCubic(double t) { return t * t * t; }
        inline double easeOutCubic(double t) { return (--t) * t * t + 1; }
        inline double easeInOutCubic(double t) { return t < 0.5 ? 4 * t * t * t : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1; }
        inline double easeInQuart(double t) { return t * t * t * t; }
        inline double easeOutQuart(double t) { return 1 - (--t) * t * t * t; }
        inline double easeInOutQuart(double t) { return t < 0.5 ? 8 * t * t * t * t : 1 - 8 * (--t) * t * t * t; }
        inline double easeInQuint(double t) { return t * t * t * t * t; }
        inline double easeOutQuint(double t) { return 1 + (--t) * t * t * t * t; }
        inline double easeInOutQuint(double t) { return t < 0.5 ? 16 * t * t * t * t * t : 1 + 16 * (--t) * t * t * t * t; }
        inline double easeInSine(double t) { return 1 - cos(t * PI / 2); }
        inline double easeOutSine(double t) { return sin(t * PI / 2); }
        inline double easeInOutSine(double t) { return 0.5 * (1 - cos(PI * t)); }
        inline double easeInExpo(double t) { return t == 0 ? 0 : pow(2, 10 * (t - 1)); }
        inline double easeOutExpo(double t) { return t == 1 ? 1 : 1 - pow(2, -10 * t); }
        inline double easeInOutExpo(double t) { return t == 0 ? 0 : t == 1 ? 1 : t < 0.5 ? 0.5 * pow(2, 20 * t - 10) : 1 - 0.5 * pow(2, -20 * t + 10); }
        inline double easeInCirc(double t) { return 1 - sqrt(1 - t * t); }
        inline double easeOutCirc(double t) { return sqrt(1 - (--t) * t); }
        inline double easeInOutCirc(double t) { return t < 0.5 ? 0.5 * (1 - sqrt(1 - 4 * t * t)) : 0.5 * (sqrt(-((2 * t) - 3) * ((2 * t) - 1)) + 1); }
        inline double easeInBack(double t) { return t * t * (2.70158 * t - 1.70158); }
        inline double easeOutBack(double t) { return 1 + (--t) * t * (2.70158 * t + 1.70158); }
        inline double easeInOutBack(double t) { return t < 0.5 ? 8 * t * t * t * (3 * t - 2) : 1 + 8 * (--t) * t * t * (3 * t + 2); }
        inline double easeInElastic(double t) { return (t == 0 || t == 1) ? t : -pow(2, 10 * t - 10) * sin((t * 10 - 10.75) * (2 * PI / 3)); }
        inline double easeOutElastic(double t) { return (t == 0 || t == 1) ? t : pow(2, -10 * t) * sin((t * 10 - 0.75) * (2 * PI / 3)) + 1; }
        inline double easeInOutElastic(double t) { return (t == 0 || t == 1) ? t : t < 0.5 ? -0.5 * pow(2, 20 * t - 10) * sin((20 * t - 11.125) * (2 * PI / 4.5)) : pow(2, -20 * t + 10) * sin((20 * t - 11.125) * (2 * PI / 4.5)) * 0.5 + 1; }
        inline double easeOutBounce(double t) { if (t < 1 / 2.75) { return 7.5625 * t * t; } else if (t < 2 / 2.75) { return 7.5625 * (t -= 1.5 / 2.75) * t + 0.75; } else if (t < 2.5 / 2.75) { return 7.5625 * (t -= 2.25 / 2.75) * t + 0.9375; } else { return 7.5625 * (t -= 2.625 / 2.75) * t + 0.984375; } }
        inline double easeInBounce(double t) { return 1 - easeOutBounce(1 - t); }
        inline double easeInOutBounce(double t) { return t < 0.5 ? 0.5 * easeInBounce(t * 2) : 0.5 * easeOutBounce(t * 2 - 1) + 0.5; }

        static const std::function<double(double)> getEasingFunction(EasingFunction easingFunction)
        {
            static std::unordered_map<EasingFunction, std::function<double(double)>> easingFunctions = {
                {EasingFunction::Linear, linear},
                {EasingFunction::InQuad, easeInQuad},
                {EasingFunction::OutQuad, easeOutQuad},
                {EasingFunction::InOutQuad, easeInOutQuad},
                {EasingFunction::InCubic, easeInCubic},
                {EasingFunction::OutCubic, easeOutCubic},
                {EasingFunction::InOutCubic, easeInOutCubic},
                {EasingFunction::InQuart, easeInQuart},
                {EasingFunction::OutQuart, easeOutQuart},
                {EasingFunction::InOutQuart, easeInOutQuart},
                {EasingFunction::InQuint, easeInQuint},
                {EasingFunction::OutQuint, easeOutQuint},
                {EasingFunction::InOutQuint, easeInOutQuint},
                {EasingFunction::InSine, easeInSine},
                {EasingFunction::OutSine, easeOutSine},
                {EasingFunction::InOutSine, easeInOutSine},
                {EasingFunction::InExpo, easeInExpo},
                {EasingFunction::OutExpo, easeOutExpo},
                {EasingFunction::InOutExpo, easeInOutExpo},
                {EasingFunction::InCirc, easeInCirc},
                {EasingFunction::OutCirc, easeOutCirc},
                {EasingFunction::InOutCirc, easeInOutCirc},
                {EasingFunction::InBack, easeInBack},
                {EasingFunction::OutBack, easeOutBack},
                {EasingFunction::InOutBack, easeInOutBack},
                {EasingFunction::InElastic, easeInElastic},
                {EasingFunction::OutElastic, easeOutElastic},
                {EasingFunction::InOutElastic, easeInOutElastic},
                {EasingFunction::InBounce, easeInBounce},
                {EasingFunction::OutBounce, easeOutBounce},
                {EasingFunction::InOutBounce, easeInOutBounce}
            };

            return easingFunctions[easingFunction];
        }
    }
}
