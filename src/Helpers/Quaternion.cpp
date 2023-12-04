#include "Quaternion.h"

#include <cmath>

#include "GeneralHelper.h"

Quaternion::Quaternion(uint8_t type, Vector3u16 vector)
{
    float coefficient_1 = static_cast<float>(vector.x) / QUATERNION_PRECISION_MULT;
    float coefficient_2 = static_cast<float>(vector.y) / QUATERNION_PRECISION_MULT;
    float coefficient_3 = static_cast<float>(vector.z) / QUATERNION_PRECISION_MULT;
    float coefficient_4 = std::sqrt(1 - (std::pow(coefficient_1, 2) + std::pow(coefficient_2, 2) + std::pow(coefficient_3, 2)));
    switch (type)
    {
    case 0:
        x = coefficient_4;
        y = coefficient_1;
        z = coefficient_2;
        w = coefficient_3;
        break;
    case 1:
        x = coefficient_1;
        y = coefficient_4;
        z = coefficient_2;
        w = coefficient_3;
        break;
    case 2:
        x = coefficient_1;
        y = coefficient_2;
        z = coefficient_4;
        w = coefficient_3;
        break;
    case 3:
        x = coefficient_1;
        y = coefficient_2;
        z = coefficient_3;
        w = coefficient_4;
        break;
    default:
        x = (type == 4 ? 1 : 0);
        y = (type == 5 ? 1 : 0);
        z = (type == 6 ? 1 : 0);
        w = (type == 7 ? 1 : 0);
        break;
    }
}

std::pair<uint8_t, float> Quaternion::getTypeAndSign()
{
    uint8_t type = 0;
    float value = std::abs(x);
    float sign = 1;
    if (std::abs(y) > value)
    {
        type = 1;
        value = y;
        sign = (y < 0 ? -1 : 1);
    }
    if (std::abs(z) > value)
    {
        type = 2;
        value = z;
        sign = (z < 0 ? -1 : 1);
    }
    if (std::abs(w) > value)
    {
        type = 3;
        value = w;
        sign = (w < 0 ? -1 : 1);
    }
    if (floatsAreEqual(value, 1))
    {
        type += 4;
        value = y;
        sign = (y < 0 ? -1 : 1);
    }
    return {type, sign};
}

Vector3u16 Quaternion::getVector(std::pair<uint8_t, float> type_and_sign)
{
    Vector3u16 vec;
    switch (type_and_sign.first)
    {
    case 0:
        vec.x = static_cast<uint16_t>(y * type_and_sign.second * QUATERNION_PRECISION_MULT);
        vec.y = static_cast<uint16_t>(z * type_and_sign.second * QUATERNION_PRECISION_MULT);
        vec.z = static_cast<uint16_t>(w * type_and_sign.second * QUATERNION_PRECISION_MULT);
        break;
    case 1:
        vec.x = static_cast<uint16_t>(x * type_and_sign.second * QUATERNION_PRECISION_MULT);
        vec.y = static_cast<uint16_t>(z * type_and_sign.second * QUATERNION_PRECISION_MULT);
        vec.z = static_cast<uint16_t>(w * type_and_sign.second * QUATERNION_PRECISION_MULT);
        break;
    case 2:
        vec.x = static_cast<uint16_t>(x * type_and_sign.second * QUATERNION_PRECISION_MULT);
        vec.y = static_cast<uint16_t>(y * type_and_sign.second * QUATERNION_PRECISION_MULT);
        vec.z = static_cast<uint16_t>(w * type_and_sign.second * QUATERNION_PRECISION_MULT);
        break;
    default:
        vec.x = static_cast<uint16_t>(x * type_and_sign.second * QUATERNION_PRECISION_MULT);
        vec.y = static_cast<uint16_t>(y * type_and_sign.second * QUATERNION_PRECISION_MULT);
        vec.z = static_cast<uint16_t>(z * type_and_sign.second * QUATERNION_PRECISION_MULT);
        break;
    }
    return vec;
}
