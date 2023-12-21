#include "Groups.h"

#include "Server.h"

void Groups::createGroup(std::shared_ptr<Player> player)
{
    std::shared_ptr<Group> group = std::make_shared<Group>();
    group->index = groups.size();
    group->size = player->client.group_size;
    group->auto_squad = player->client.auto_squad;
    player->group = group;
    groups.push_back(group);
}

std::vector<std::shared_ptr<Group>>::iterator Groups::findFreeGroup(std::shared_ptr<Player> player)
{
    for (auto group = groups.begin(); group != groups.end(); ++group)
    {
        if ((*group)->auto_squad && player->client.group_size <= server->preferences->group_size - (*group)->size)
            return group;
    }
    return groups.end();
}

void Groups::addPlayerToGroup(std::shared_ptr<Player> player)
{
    if (!player->group)
    {
        auto group = findFreeGroup(player);
        if (player->client.auto_squad && group != groups.end())
            player->group = *group;
        else
            createGroup(player);
    }
}
