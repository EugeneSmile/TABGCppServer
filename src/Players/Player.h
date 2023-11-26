#ifndef PLAYERS_PLAYER_H_GUARD
#define PLAYERS_PLAYER_H_GUARD

#include <string>
#include <cstdint>

#include "Types.h"

struct Player
{
    uint8_t id;
    uint8_t group;
    std::string name;
    Vector3f location;
    Player(uint8_t id, uint8_t group, std::string name);
};

#endif