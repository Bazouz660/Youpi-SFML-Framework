/*
** EPITECH PROJECT, 2023
** RPG3
** File description:
** numberGenerator
*/

#ifndef NUMBERGENERATOR_HPP_
    #define NUMBERGENERATOR_HPP_

    namespace ypi {
        class nbgen {
            public:
                static int between(int min, int max);
                static float between(float min, float max);
                static bool boolean();
            private:
                nbgen();
        };
    }

#endif /* !NUMBERGENERATOR_HPP_ */
