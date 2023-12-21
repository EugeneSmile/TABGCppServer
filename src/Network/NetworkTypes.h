#ifndef NETWORK_NETWORKTYPES_H_GUARD
#define NETWORK_NETWORKTYPES_H_GUARD

#include <cstdint>
#include <memory>

#include "Types.h"
#include "Weapon.h"

struct NPlayerLeft
{
    uint8_t player_index;
    bool player_destroy;
};

struct NWeaponPickup
{
    uint8_t player_index;
    uint32_t weapon_index;
    std::shared_ptr<Weapon> weapon;
    uint8_t slot;
};

#endif