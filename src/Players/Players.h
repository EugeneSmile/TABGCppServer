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
    std::vector<Player> players;
    std::vector<uint8_t> connected;

    std::vector<Player>::iterator addPlayer(PlayerServiceData &service_data);
    std::vector<Player>::iterator removePlayer(const uint32_t peer_id);
    std::vector<Player>::iterator findPlayer(const std::string &playfab_id);
    std::vector<Player>::iterator findPlayer(const uint8_t game_index);
    std::vector<Player>::iterator findPlayer(const uint32_t peer_id);
    std::vector<Player>::iterator findPlayer(const uint64_t session_id);
};

#endif