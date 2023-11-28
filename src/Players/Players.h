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
    std::unordered_map<uint64_t, uint8_t> game_index;
    std::unordered_map<uint32_t, uint64_t> net_session_id;
    std::unordered_map<uint64_t, LoginData> login_data;

public:
    void addPlayer(const uint32_t net_id, const LoginData &data);
    void removePlayer(const uint32_t net_id);
    void updatePlayerLocation(uint8_t id, Vector3f new_location);
    Player &getPlayer(const uint8_t id);
    Player &getPlayer(const uint64_t key);
    uint8_t getPlayersCount();
    uint8_t getGameIndex(uint64_t key);
    LoginData &getLoginData(uint64_t key);
};

#endif