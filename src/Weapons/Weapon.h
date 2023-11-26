#ifndef WEAPONS_WEAPON_H_GUARD
#define WEAPONS_WEAPON_H_GUARD

#include <cstdint>

#include "Types.h"

struct Weapon
{
    uint32_t id;
    uint32_t type;
    uint32_t count;
    Vector3f location;
    Weapon(uint32_t id, uint32_t type, uint32_t count, Vector3f location);
};

#endif