#ifndef GAME_CARS_CARS_H_GUARD
#define GAME_CARS_CARS_H_GUARD

#include <vector>
#include <cstdint>

#include "ServerPtr.h"
#include "Car.h"
#include "Types.h"

class Cars : public ServerPtr
{
public:
    std::vector<std::shared_ptr<Car>> cars;
    Cars();
    void addCar(uint32_t seats, uint32_t id, Vector3f position);
};

#endif