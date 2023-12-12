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
    uint8_t index;
    RingDataType data_type;
    Vector3f center;
    float size;
    std::chrono::duration<float> travelled_time;
};

#endif