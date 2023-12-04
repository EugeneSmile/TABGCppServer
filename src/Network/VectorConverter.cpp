#include "VectorConverter.h"

#include <limits>
#include <cmath>

Vector3f convertUInt8ToFloat(const Vector3u8 &input)
{
    return {
        (input.x - DIRECTION_PRECISION_MULT) / DIRECTION_PRECISION_MULT,
        (input.y - DIRECTION_PRECISION_MULT) / DIRECTION_PRECISION_MULT,
        (input.z - DIRECTION_PRECISION_MULT) / DIRECTION_PRECISION_MULT};
}

Vector3u8 convertFloatToUInt8(const Vector3f &input)
{
    return {
        input.x * DIRECTION_PRECISION_MULT + DIRECTION_PRECISION_MULT,
        input.y * DIRECTION_PRECISION_MULT + DIRECTION_PRECISION_MULT,
        input.z * DIRECTION_PRECISION_MULT + DIRECTION_PRECISION_MULT};
}
