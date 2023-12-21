#include "Cars.h"

Cars::Cars()
{
    cars.resize(0);
}

void Cars::addCar(uint32_t seats, uint32_t id, Vector3f position)
{
    uint8_t index = cars.size();
    cars.push_back(std::make_shared<Car>(index, seats, id, position));
}