#ifndef HELPERS_QUATERNION_H_GUARD
#define HELPERS_QUATERNION_H_GUARD

#include <cstdint>

#include "Types.h"

constexpr float QUATERNION_PRECISION_MULT = 10000;

class Quaternion
{
public:
    float x{0};
    float y{0};
    float z{0};
    float w{0};
    Quaternion(uint8_t type = 3, Vector3u16 vector = {});
    std::pair<uint8_t, float> getTypeAndSign();
    Vector3u16 getVector(std::pair<uint8_t, float> type_and_sign);
};

#endif