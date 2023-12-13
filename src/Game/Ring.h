#ifndef GAME_RING_H_GUARD
#define GAME_RING_H_GUARD

#include <chrono>

#include "Types.h"

enum class RingDataType
{
    FlyingTime = 0,
    NextRingData = 1,
    StartMove = 2
};

struct Ring
{
    RingDataType data_type;
    uint8_t index;
    Vector3f center;
    float size;
    float progress;
    std::chrono::duration<float> travel_time;
};

#endif