#include "Car.h"

#include "Player.h"

Car::Car(uint32_t index, uint32_t number_of_seats, uint32_t id, Vector3f position)
    : index(index), id(id), position(position)
{
    seats.resize(number_of_seats, nullptr);
}
