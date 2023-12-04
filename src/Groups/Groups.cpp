#include "Groups.h"

#include "Server.h"

uint8_t Groups::createGroup(PlayerServiceData &data)
{
    GroupData group_data;
    group_data.auto_squad = data.auto_squad;
    group_data.size = data.squad_members;
    groups.push_back(group_data);
    player_group_map.emplace(data.game_index, groups.size() - 1);
    return groups.size() - 1;
}

std::vector<GroupData>::iterator Groups::findFreeGroup(PlayerServiceData &data)
{
    for (auto group = groups.begin(); group != groups.end(); ++group)
    {
        if (group->auto_squad && data.squad_members <= server->preferences->group_size - group->size)
            return group;
    }
    return groups.end();
}

uint8_t Groups::addPlayerToGroup(PlayerServiceData &data)
{
    if (!player_group_map.contains(data.game_index))
    {
        auto group = findFreeGroup(data);
        if (data.auto_squad && group != groups.end())
            player_group_map.emplace(data.game_index, group - groups.begin());
        else
            createGroup(data);
    }
    return player_group_map.at(data.game_index);
}
