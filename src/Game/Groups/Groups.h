#ifndef GAME_GROUPS_GROUPS_H_GUARD
#define GAME_GROUPS_GROUPS_H_GUARD

#include <vector>
#include <memory>

#include "Group.h"
#include "ServerPtr.h"
#include "Player.h"

class Groups : public ServerPtr
{
private:
    void createGroup(std::shared_ptr<Player> player);
    std::vector<std::shared_ptr<Group>>::iterator findFreeGroup(std::shared_ptr<Player> player);

public:
    std::vector<std::shared_ptr<Group>> groups;
    void addPlayerToGroup(std::shared_ptr<Player> player);
};

#endif