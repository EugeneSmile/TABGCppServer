#ifndef PLAYERS_PLAYERS_H_GUARD
#define PLAYERS_PLAYERS_H_GUARD

#include <vector>
#include <cstdint>

#include "ServerPtr.h"
#include "Types.h"
#include "Player.h"

class Players : public ServerPtr
{
private:
    std::vector<Player> players;

public:
    void addPlayer(const Player &player);
    void removePlayer(const size_t id);
    void updatePlayerLocation(uint8_t id, Vector3f new_location);
    Player &getPlayer(const size_t id);
    uint8_t getNextFreeID();
    uint8_t getPlayersCount();
};

#endif