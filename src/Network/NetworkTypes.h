#ifndef NETWORK_NETWORKTYPES_H_GUARD
#define NETWORK_NETWORKTYPES_H_GUARD

#include <cstdint>

#include "Types.h"

struct NQuaternion
{
    uint8_t index;
    Vector3u8 coefficients;
};

struct NInitRoom
{
    uint8_t player_game_id;
    uint8_t group_game_id;
};

struct NPlayerLeft
{
    uint8_t player_game_id;
    bool player_destroy;
};

struct NWeaponPickup
{
    uint8_t player_game_id;
    uint32_t weapon_type_id;
    uint32_t weapon_spawn_id;
    uint32_t weapon_quantity;
    uint8_t slot;
};

#endif