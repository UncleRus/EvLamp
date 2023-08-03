#include "utils.h"

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (out_max - out_min) * (x - in_min) / (in_max - in_min) + out_min;
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (out_max - out_min) * (x - in_min) / (in_max - in_min) + out_min;
}
