#ifndef WEAPONS_WEAPONS_H_GUARD
#define WEAPONS_WEAPONS_H_GUARD

#include <vector>
#include <cstdint>

#include "ServerPtr.h"
#include "Weapon.h"

class Weapons : public ServerPtr
{
private:
    std::vector<Weapon> weapons;

public:
    Weapon &getWeapon(uint32_t id);
    void spawnWeapon(Weapon weapon);
    void removeWeapon(uint32_t id);
    uint32_t getNumberOfWeapons();
};

#endif