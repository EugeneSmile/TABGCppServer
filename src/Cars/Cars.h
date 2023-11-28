#ifndef CARS_CARS_H_GUARD
#define CARS_CARS_H_GUARD

#include <vector>
#include <cstdint>

#include "ServerPtr.h"
#include "Car.h"

class Cars : public ServerPtr
{
private:
    std::vector<Car> cars;

public:
    Car &getCar(uint32_t id);
    void spawnCar(Car car);
    void removeCar(uint32_t id);
    uint32_t getNumberOfCars();
};

#endif