#ifndef GAME_CARS_CAR_H_GUARD
#define GAME_CARS_CAR_H_GUARD

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

#include "Types.h"
#include "Enums.h"
#include "Quaternion.h"
#include "Part.h"

class Player;
struct Car
{
    uint32_t index;
    std::vector<std::shared_ptr<Player>> seats;
    uint32_t id;
    Vector3f position;
    Quaternion rotation;
    Vector3f input{0, 0, 0};
    std::vector<Part> parts;
    CarDrivingState driving_state{CarDrivingState::None};
    Car(uint32_t index, uint32_t number_of_seats, uint32_t id, Vector3f position);
};

#endif