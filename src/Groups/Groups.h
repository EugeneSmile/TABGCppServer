#ifndef GROUPS_GROUPS_H_GUARD
#define GROUPS_GROUPS_H_GUARD

#include <vector>
#include <unordered_map>

#include "Group.h"
#include "ServerPtr.h"
#include "Player.h"

class Groups : public ServerPtr
{
private:
    uint8_t createGroup(PlayerServiceData &data);
    std::vector<GroupData>::iterator findFreeGroup(PlayerServiceData &data);

public:
    std::unordered_map<uint8_t, uint8_t> player_group_map;
    std::vector<GroupData> groups;
    uint8_t addPlayerToGroup(PlayerServiceData &data);
};

#endif