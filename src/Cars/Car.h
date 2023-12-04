#ifndef CARS_CAR_H_GUARD
#define CARS_CAR_H_GUARD

#include <cstdint>
#include <vector>
#include <string>

#include "Types.h"
#include "Enums.h"
#include "Quaternion.h"

struct CarPart
{
    float health;
    std::string name;
};

struct Car
{
    uint32_t seats;
    uint32_t type;
    Vector3f position;
    Quaternion rotation;
    Vector3f input{0, 0, 0};
    std::vector<CarPart> parts;
    CarDrivingState driving_state{CarDrivingState::None};
    Car(uint32_t seats, uint32_t type, Vector3f position);
};

#endif