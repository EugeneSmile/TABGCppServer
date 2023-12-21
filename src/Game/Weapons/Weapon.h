#ifndef GAME_WEAPONS_WEAPON_H_GUARD
#define GAME_WEAPONS_WEAPON_H_GUARD

#include <cstdint>

#include "Types.h"
#include "Enums.h"

/* enum class WeaponType
{
    None,
    Weapon,
    Ammo,
    Scope,
    Barrel,
    Utility,
    Grenade,
    Heal,
    Spell,
    Blessing,
    Shield,
    Helmet,
    Vest,
    Other
};
 */
struct Weapon
{
    uint32_t index;
    uint32_t id;
    WeaponType type;
    uint32_t quantity;
    Vector3f position;
    float probability;
    int32_t spawn_ammo_id;
    int32_t spawn_ammo_size;
    std::string name;
    Weapon(uint32_t index, uint32_t id, uint32_t quantity, Vector3f position);
};

#endif