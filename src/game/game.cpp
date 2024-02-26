
#include "game.h"

float cos(float x)
{
    return _FSin(x, 1);
}

float sin(float x)
{
    return _FSin(x, 0);
}

float CalculateTimeFraction(int startTime, int duration)
{
    if (duration < 1)
        return 1.0;

    int deltaTime = Sys_Milliseconds() - startTime;

    if (deltaTime >= duration)
        return 1.0;

    if (deltaTime < 1)
        return 0.0;

    return static_cast<float>(deltaTime) / static_cast<float>(duration);
}