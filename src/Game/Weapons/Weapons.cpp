#include "Weapons.h"

void Weapons::addWeapon(uint32_t id, uint32_t quantity, Vector3f position)
{
    uint8_t index = weapons.size();
    weapons.push_back(std::make_shared<Weapon>(index, id, quantity, position));
}