#ifndef WEAPONS_WEAPON_H_GUARD
#define WEAPONS_WEAPON_H_GUARD

#include <cstdint>

#include "Types.h"

struct Weapon
{
    uint32_t type;
    uint32_t quantity;
    Vector3f position;
};

#endif