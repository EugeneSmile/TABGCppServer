#ifndef CARS_CARS_H_GUARD
#define CARS_CARS_H_GUARD

#include <vector>
#include <cstdint>

#include "ServerPtr.h"
#include "Car.h"
#include "Types.h"

class Cars : public ServerPtr
{
public:
    std::vector<std::pair<CarServiceData, Car>> cars;
    std::vector<std::pair<CarServiceData, Car>>::iterator findCar(const uint8_t game_index);
    std::vector<std::pair<CarServiceData, Car>>::iterator addCar(Car car);
    void removeCar(const uint8_t game_index);
};

#endif