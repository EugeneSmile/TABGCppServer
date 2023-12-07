#include "VectorConverter.h"

#include <limits>
#include <cmath>

Vector3f convertUInt8ToFloat(const Vector3u8 &input)
{
    return {
        (static_cast<float>(input.x) - DIRECTION_PRECISION_MULT) / DIRECTION_PRECISION_MULT,
        (static_cast<float>(input.y) - DIRECTION_PRECISION_MULT) / DIRECTION_PRECISION_MULT,
        (static_cast<float>(input.z) - DIRECTION_PRECISION_MULT) / DIRECTION_PRECISION_MULT};
}

Vector3u8 convertFloatToUInt8(const Vector3f &input)
{
    return {
        static_cast<uint8_t>(input.x * DIRECTION_PRECISION_MULT + DIRECTION_PRECISION_MULT),
        static_cast<uint8_t>(input.y * DIRECTION_PRECISION_MULT + DIRECTION_PRECISION_MULT),
        static_cast<uint8_t>(input.z * DIRECTION_PRECISION_MULT + DIRECTION_PRECISION_MULT)};
}
