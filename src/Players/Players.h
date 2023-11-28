#ifndef PLAYERS_PLAYERS_H_GUARD
#define PLAYERS_PLAYERS_H_GUARD

#include <unordered_map>
#include <vector>
#include <cstdint>

#include "ServerPtr.h"
#include "Types.h"
#include "Player.h"

class Players : public ServerPtr
{
private:
    std::vector<Player> players;
    std::vector<uint8_t> in_game;
    std::unordered_map<uint64_t, uint8_t> indexes;

public:
    void addPlayer(const uint64_t key, const Player &player);
    void removePlayer(const uint8_t id);
    void updatePlayerLocation(uint8_t id, Vector3f new_location);
    Player &getPlayer(const uint8_t id);
    uint8_t getNextFreeID();
    uint8_t getPlayersCount();
};

#endif