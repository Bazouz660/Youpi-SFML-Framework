/*
** EPITECH PROJECT, 2023
** RPG3
** File description:
** numberGenerator
*/

#include "numberGenerator.hpp"
#include <random>
#include <ctime>

namespace exng
{

nbgen::nbgen()
{
    srand(time(NULL));
}

int nbgen::between(int minNum, int maxNum)
{
    int result = 0;
    int lowNum = 0;
    int hiNum = 0;

    if (minNum < maxNum) {
        lowNum = minNum;
        hiNum = maxNum + 1;
    } else {
        lowNum = maxNum + 1;
        hiNum = minNum;
    }
    result = (rand() % (hiNum - lowNum)) + lowNum;
    return result;
}

float nbgen::between(float minNum, float maxNum)
{
    float result = 0.0;
    float lowNum = 0.0;
    float hiNum = 0.0;

    if (minNum < maxNum) {
        lowNum = minNum;
        hiNum = maxNum;
    } else {
        lowNum = maxNum;
        hiNum = minNum;
    }
    float range = hiNum - lowNum;
    result = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * range + lowNum;
    return result;
}

bool nbgen::boolean()
{
    return nbgen::between(0, 1);
}

}