#include "Cars.h"

std::vector<std::pair<CarServiceData, Car>>::iterator Cars::findCar(const uint8_t game_index)
{
    for (auto it = cars.begin(); it != cars.end(); ++it)
        if (it->first.game_index == game_index)
            return it;
    return cars.end();
}

std::vector<std::pair<CarServiceData, Car>>::iterator Cars::addCar(Car car)
{
    return cars.end();
}

void Cars::removeCar(const uint8_t game_index)
{
    auto car = findCar(game_index);
    if (car != cars.end())
        cars.erase(car);
}
