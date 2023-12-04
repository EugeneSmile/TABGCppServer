#ifndef NETWORK_VECTORCONVERTER_H_GUARD
#define NETWORK_VECTORCONVERTER_H_GUARD

#include "Types.h"
#include "NetworkTypes.h"

constexpr float DIRECTION_PRECISION_MULT = 100;

Vector3f convertUInt8ToFloat(const Vector3u8 &input);
Vector3u8 convertFloatToUInt8(const Vector3f &input);

#endif