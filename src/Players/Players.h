#ifndef PLAYERS_PLAYERS_H_GUARD
#define PLAYERS_PLAYERS_H_GUARD

#include <unordered_map>
#include <cstdint>

#include "ServerPtr.h"
#include "Types.h"
#include "Player.h"

class Players : public ServerPtr
{
private:
    std::unordered_map<uint8_t, Player> players;
    uint8_t last_id = 0;

public:
    void addPlayer(const Player &player);
    void removePlayer(const Player &player);
    void updatePlayerLocation(uint8_t player_id, Vector3f new_location);
    uint8_t increaseLastId();
};

#endif