#ifndef WEAPONS_WEAPONS_H_GUARD
#define WEAPONS_WEAPONS_H_GUARD

#include <unordered_map>
#include <cstdint>

#include "ServerPtr.h"
#include "Weapon.h"

class Weapons : public ServerPtr
{
private:
    std::unordered_map<uint32_t, Weapon> weapons;
    uint32_t current_id = 0;

public:
    void spawnWeapon(Weapon weapon);
    void removeWeapon(Weapon weapon);
};

#endif