#include "Weapons.h"

void Weapons::spawnWeapon(Weapon weapon)
{
    weapons.try_emplace(weapon.id, weapon);
}
void Weapons::removeWeapon(Weapon weapon)
{
    weapons.erase(weapon.id);
}
