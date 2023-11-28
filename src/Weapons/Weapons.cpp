#include "Weapons.h"

Weapon &Weapons::getWeapon(uint32_t id)
{
    return weapons.at(id);
}

void Weapons::spawnWeapon(Weapon weapon)
{
    weapons.push_back(weapon);
}
void Weapons::removeWeapon(uint32_t id)
{
    weapons.erase(std::next(weapons.begin(), id));
}

uint32_t Weapons::getNumberOfWeapons()
{
    return weapons.size();
}