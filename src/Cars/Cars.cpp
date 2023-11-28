#include "Cars.h"

Car &Cars::getCar(uint32_t id)
{
    return cars.at(id);
}

void Cars::spawnCar(Car car)
{
    cars.push_back(car);
}
void Cars::removeCar(uint32_t id)
{
    cars.erase(std::next(cars.begin(), id));
}

uint32_t Cars::getNumberOfCars()
{
    return cars.size();
}