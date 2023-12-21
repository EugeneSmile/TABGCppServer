#ifndef GAME_GROUPS_GROUP_H_GUARD
#define GAME_GROUPS_GROUP_H_GUARD

#include <vector>
#include <cstdint>

struct Group
{
    uint8_t index;
    uint8_t size;
    bool auto_squad{false};
};

#endif