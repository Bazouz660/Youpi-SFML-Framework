/*
** EPITECH PROJECT, 2023
** RPG3
** File description:
** color
*/

#ifndef COLOR_HPP_
    #define COLOR_HPP_

    #include "../../../lib_headers/common.hpp"
    #include <cmath>

    namespace ypi {
        namespace coloring {
            static sf::Color mix(sf::Color c1, sf::Color c2, double prog)
            {
                float diff_r = c1.r - c2.r;
                float diff_g = c1.g - c2.g;
                float diff_b = c1.b - c2.b;
                float diff_a = c1.a - c2.a;
                sf::Color res = sf::Color(0, 0, 0, 0);

                if (prog > 1)
                    prog = 1;
                if (prog < 0)
                    prog = 0;
                res.r = c1.r + (diff_r * prog);
                res.g = c1.g + (diff_g * prog);
                res.b = c1.b + (diff_b * prog);
                res.a = c1.a + (diff_a * prog);
                return res;
            }

            static sf::Color randomColor()
            {
                sf::Color res = sf::Color(0, 0, 0, 0);

                res.r = rand() % 255;
                res.g = rand() % 255;
                res.b = rand() % 255;
                res.a = 255;
                return res;
            }

            static sf::Color rainbow(float t)
            {
                sf::Color res = sf::Color(0, 0, 0, 0);

                res.r = (std::sin(t) * 127 + 128);
                res.g = (std::sin(t + 2) * 127 + 128);
                res.b = (std::sin(t + 4) * 127 + 128);
                res.a = 255;
                return res;
            }
        }
    }

#endif /* !COLOR_HPP_ */
