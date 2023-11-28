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
public:
    std::vector<std::pair<PlayerServiceData, Player>> players;
    std::vector<uint8_t> connected;

    std::vector<std::pair<PlayerServiceData, Player>>::iterator addPlayer(const uint32_t net_id, const LoginData &data);
    uint64_t removePlayer(const uint32_t net_id);
    void updatePlayerLocation(uint8_t id, Vector3f new_location);
    std::vector<std::pair<PlayerServiceData, Player>>::iterator findPlayer(const uint8_t game_index);
    std::vector<std::pair<PlayerServiceData, Player>>::iterator findPlayer(const uint32_t net_id);
    std::vector<std::pair<PlayerServiceData, Player>>::iterator findPlayer(const uint64_t key);
};

#endif