#ifndef CARS_CAR_H_GUARD
#define CARS_CAR_H_GUARD

#include <cstdint>
#include <vector>
#include <string>

#include "Types.h"
#include "Enums.h"

// Need to clarify
enum CarRotationEnum
{
    NOROTATION = 0,
    X_NEG = 4,
    X_POS = 5,
    Y_NEG = 6,
    Y_POS = 7
};

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
    CarRotationEnum rotation_state{CarRotationEnum::NOROTATION};
    Vector3u16 rotation{0, 0, 0};
    Vector3u8 input{0, 0, 0};
    std::vector<CarPart> parts;
    CarDrivingState driving_state{CarDrivingState::NoDrivingState};
    Car(uint32_t seats, uint32_t type, Vector3f position);
};

#endif