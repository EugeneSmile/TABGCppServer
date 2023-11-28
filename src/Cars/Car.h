#ifndef CARS_CAR_H_GUARD
#define CARS_CAR_H_GUARD

#include <cstdint>
#include <vector>
#include <string>

#include "Types.h"

struct CarPart
{
    float health;
    std::string name;
};

struct Car
{
    uint32_t seats;
    uint32_t i2;
    Vector3f location;
    std::vector<CarPart> parts;
    Car(uint32_t seats, uint32_t i2, Vector3f location);
};

#endif